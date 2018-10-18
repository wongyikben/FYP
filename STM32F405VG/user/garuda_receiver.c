#include "garuda_controller.h"

static const s32 GAR_DATA_SIZE = 16;
static const s32 GAR_JOY_DEAD = 0xC0;
static const s32 GAR_JOY_MAX = 2047;
static const s32 GAR_JOY_SCALE = 1000;

void spi_gar_rx_handler(u8 data);
void bt_gar_rx_handler(u8 data);

static GarPriority garuda_priority = GAR_BT_FIRST;

static uint32_t pressedTime[GAR_BTN_COUNT] = {0};
static OnClickListener clickListeners[GAR_BTN_COUNT] = {0};
static OnReleaseListener releaseListeners[GAR_BTN_COUNT] = {0};
static HoldListenerStruct holdListeners[GAR_BTN_COUNT] = {0};

static void spi_gar_update_ssi() {
	if (GPIO_ReadInputDataBit(gar_nss->gpio, gar_nss->gpio_pin)) {
		SPI_NSSInternalSoftwareConfig(SPI_GAR_PORT, SPI_NSSInternalSoft_Set);
	} else {
		SPI_NSSInternalSoftwareConfig(SPI_GAR_PORT, SPI_NSSInternalSoft_Reset);
	}
}

void garuda_transmit_init(GarPriority priority);
void garuda_tft_init(void);

void garuda_init(GarPriority priority){
	garuda_priority = priority;
	
	//SPI init
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	spi_init(SPI_GAR_PORT, SPI_Mode_Slave, SPI_BaudRatePrescaler_64);
	
	gpio_rcc_init(gar_nss);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	gpio_input_init(gar_nss, GPIO_PuPd_UP);
	
	SYSCFG_EXTILineConfig(GAR_NSS_PORTSOURCE, GAR_NSS_PINSOURCE);
	
	EXTI_InitStructure.EXTI_Line = GAR_NSS_EXTILINE;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = GAR_NSS_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	spi_rx_set_handler(SPI_GAR_PORT, spi_gar_rx_handler);
	spi_gar_update_ssi();
	
	//Bluetooth init
	uart_init(BT_GAR_COM, 115200);
	uart_interrupt_init(BT_GAR_COM, bt_gar_rx_handler);
	
	//CRC init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
	CRC_ResetDR();
	
	garuda_transmit_init(priority);
	garuda_tft_init();
}

//Resolve the joystick by calculating the dead zone
static s32 resolve_deadzone(s32 raw){
	
	raw = GAR_JOY_MAX - raw;
	
	if (raw >= -GAR_JOY_DEAD && raw <= GAR_JOY_DEAD) {
		return 0;
	} else if (raw < -GAR_JOY_MAX) {
		return -GAR_JOY_SCALE;
	} else if (raw > GAR_JOY_MAX) {
		return GAR_JOY_SCALE;
	} else {
		if (raw > 0) {
			return (raw - GAR_JOY_DEAD) * GAR_JOY_SCALE / (GAR_JOY_MAX - GAR_JOY_DEAD);
		} else {
			return (raw - -GAR_JOY_DEAD) * GAR_JOY_SCALE / (GAR_JOY_MAX - GAR_JOY_DEAD);
		}
	}
}

//Return 255 if crc error, status otherwise
static u8 unpack(u8* data, s32* joy, u8* btn){
	u32 their_crc = ((data[11]&0x0F)<<28) | ((data[12]&0x7F)<<21) | ((data[13]&0x7F)<<14)  | ((data[14]&0x7F)<<7) | ((data[15]&0x7F));

	for (u8 i=10; i<16; i++){
		data[i] = i | 0x80;
	}
	
	CRC_ResetDR();
	u32 my_crc = CRC_CalcBlockCRC((u32*)data, GAR_DATA_SIZE/4);
	
	if (their_crc == my_crc){
		//ok
		
		u8 status = data[1] & 0x7F;
		
		joy[0] = resolve_deadzone((data[2]&0x7F)<<7 | (data[3]&0x7F));
		joy[1] = resolve_deadzone((data[4]&0x7F)<<7 | (data[5]&0x7F));
		
		btn[0] = data[6] & 0x01;
		btn[1] = data[6] & 0x02;
		btn[2] = data[6] & 0x04;
		btn[3] = data[6] & 0x08;
		btn[4] = data[6] & 0x10;
		btn[5] = data[6] & 0x20;
		btn[6] = data[6] & 0x40;
		
		btn[7] = data[7] & 0x01;
		btn[8] = data[7] & 0x02;
		btn[9] = data[7] & 0x04;
		btn[10] = data[7] & 0x08;
		btn[11] = data[7] & 0x10;
		btn[12] = data[7] & 0x20;
		btn[13] = data[7] & 0x40;
		
		btn[14] = data[8] & 0x01;
		btn[15] = data[8] & 0x02;
		btn[16] = data[8] & 0x04;
		btn[17] = data[8] & 0x08;
		btn[18] = data[8] & 0x10;
		btn[19] = data[8] & 0x20;
		btn[20] = data[8] & 0x40;
		
		btn[21] = data[9] & 0x01;
		btn[22] = data[9] & 0x02;
		btn[23] = data[9] & 0x04;
		btn[24] = data[9] & 0x08;
		
		return status;
	}else{
		//fucked
		return 255;
	}
}

void gar_spi_schedule_next(void);

static u8 spi_buffer[GAR_DATA_SIZE] = {0};
static u32 spi_pointer = 0;
static u32 spi_error_count = 0;
static u32 spi_last_recv = 0;
static s32 spi_joy_value[2] = {0};
static u8 spi_btn_value[GAR_BTN_COUNT] = {0};
static u8 spi_gar_status = 0;

void spi_gar_rx_handler(u8 data){
	
	if ((~data) & 0x80){
		if (data == 0){
			//continue, as start byte
			spi_pointer = 0;
		}else{
			//Escape
			return;
		}
	}
	
	spi_buffer[spi_pointer] = data;
	spi_pointer++;
	
	if (spi_pointer == GAR_DATA_SIZE){
		spi_pointer = 0;
		
		u8 status = unpack(spi_buffer, spi_joy_value, spi_btn_value);

		if (status == 255){
			//fucked
			spi_error_count++;
		}else{
			//ok
			spi_gar_status = status;
			spi_last_recv = get_ticks();
		}
	}
	
	gar_spi_schedule_next();
}

static u8 bt_buffer[GAR_DATA_SIZE] = {0};
static u32 bt_pointer = 0;
static u32 bt_error_count = 0;
static u32 bt_last_recv = 0;
static s32 bt_joy_value[2] = {0};
static u8 bt_btn_value[GAR_BTN_COUNT] = {0};
static u8 bt_gar_status = 0;

void bt_gar_rx_handler(u8 data){
	
	if ((~data) & 0x80){
		if (data == 0){
			//continue, as start byte
			bt_pointer = 0;
		}else{
			//Escape
			return;
		}
	}
	
	bt_buffer[bt_pointer] = data;
	bt_pointer++;
	
	if (bt_pointer == GAR_DATA_SIZE){
		bt_pointer = 0;
		
		u8 status = unpack(bt_buffer, bt_joy_value, bt_btn_value);
		
		if (status == 255){
			//fucked
			bt_error_count++;
		}else{
			bt_gar_status = status;
			bt_last_recv = get_ticks();
		}
	}
}

GAR_CONNECTION_MODE gar_get_connection(){
	u32 this_ticks = get_ticks();
	
	bool spi_discon = this_ticks - spi_last_recv > GAR_CONNECTION_TIMEOUT_MS;
	bool bt_discon = this_ticks - bt_last_recv > GAR_CONNECTION_TIMEOUT_MS;
	
	if (!spi_discon && !bt_discon){
		return GAR_BOTH_CONNECTED;
		
	}else if (spi_discon && bt_discon){
		spi_gar_update_ssi();
		return GAR_BOTH_DISCONNECTED;
		
	}else if(spi_discon){
		spi_gar_update_ssi();
		return GAR_ONLY_BT_CONNECTED;
		
	}else{
		return GAR_ONLY_SPI_CONNECTED;
	}
}

u8 gar_get_status(){
	//Reduce calls to gar_button_pressed
	GAR_CONNECTION_MODE connection = gar_get_connection();
	
	if (garuda_priority == GAR_SPI_FIRST){
		if (connection == GAR_BOTH_CONNECTED || connection == GAR_ONLY_SPI_CONNECTED){
			return spi_gar_status;
		}else if (connection == GAR_ONLY_BT_CONNECTED){
			return bt_gar_status;
		}else{
			return 255;
		}
	}else{
		if (connection == GAR_BOTH_CONNECTED || connection == GAR_ONLY_BT_CONNECTED){
			return bt_gar_status;
		}else if (connection == GAR_ONLY_SPI_CONNECTED){
			return spi_gar_status;
		}else{
			return 255;
		}
	}
}

u32 gar_get_spi_last_recv(){
	return spi_last_recv;
}

u32 gar_get_bt_last_recv(){
	return bt_last_recv;
}

u32 gar_get_spi_err(){
	return spi_error_count;
}

u32 gar_get_bt_err(){
	return bt_error_count;
}

s16 gar_get_joy(GarJoy j){
	if (garuda_priority == GAR_SPI_FIRST){
		if (gar_get_connection() == GAR_BOTH_CONNECTED || gar_get_connection() == GAR_ONLY_SPI_CONNECTED){
			return spi_joy_value[j];
		}else if (gar_get_connection() == GAR_ONLY_BT_CONNECTED){
			return bt_joy_value[j];
		}else{
			return 0;
		}
	}else{
		if (gar_get_connection() == GAR_BOTH_CONNECTED || gar_get_connection() == GAR_ONLY_BT_CONNECTED){
			return bt_joy_value[j];
		}else if (gar_get_connection() == GAR_ONLY_SPI_CONNECTED){
			return spi_joy_value[j];
		}else{
			return 0;
		}
	}
}

bool gar_button_pressed(GarButtonID id){
	//Reduce calls to gar_button_pressed
	GAR_CONNECTION_MODE connection = gar_get_connection();
	
	if (garuda_priority == GAR_SPI_FIRST){
		if (connection == GAR_BOTH_CONNECTED || connection == GAR_ONLY_SPI_CONNECTED){
			return spi_btn_value[id];
		}else if (connection == GAR_ONLY_BT_CONNECTED){
			return bt_btn_value[id];
		}else{
			return 0;
		}
	}else{
		if (connection == GAR_BOTH_CONNECTED || connection == GAR_ONLY_BT_CONNECTED){
			return bt_btn_value[id];
		}else if (connection == GAR_ONLY_SPI_CONNECTED){
			return spi_btn_value[id];
		}else{
			return 0;
		}
	}
}

void gar_button_update(){
	//Reduce calls to gar_button_pressed
	GAR_CONNECTION_MODE connection = gar_get_connection();
	u8* tar_btn_array;
	
	if (garuda_priority == GAR_SPI_FIRST){
		if (connection == GAR_BOTH_CONNECTED || connection == GAR_ONLY_SPI_CONNECTED){
			tar_btn_array = spi_btn_value;
		}else if (connection == GAR_ONLY_BT_CONNECTED){
			tar_btn_array = bt_btn_value;
		}
	}else{
		if (connection == GAR_BOTH_CONNECTED || connection == GAR_ONLY_BT_CONNECTED){
			tar_btn_array = bt_btn_value;
		}else if (connection == GAR_ONLY_SPI_CONNECTED){
			tar_btn_array = spi_btn_value;
		}
	}
	
	for (uint8_t i=0; i<GAR_BTN_COUNT; i++){
		if ((connection != GAR_BOTH_DISCONNECTED) && tar_btn_array[i]){
			//If the button is pressed
			
			if (pressedTime[i]==0 && clickListeners[i] != 0 ){
				//Capture the instant when the button is pressed
				clickListeners[i]();
			}
			
			if (holdListeners[i].holdListener != 0){
				if (pressedTime[i] > holdListeners[i].holdThreshold && (pressedTime[i]%holdListeners[i].trigFrequency==0)){
					//Trigger the hold listener
					holdListeners[i].holdListener();
				}
			}
			
			pressedTime[i]++;
			
		}else{
			//If the button is not pressed
			if (pressedTime[i] > 0 && releaseListeners[i] != 0){
				//Capture the instant when the button is released
				releaseListeners[i]();
			}
			pressedTime[i] = 0;
		}
	}
}

void gar_reg_OnClickListener(GarButtonID button, OnClickListener listener){
	clickListeners[button] = listener;
}

void gar_reg_OnReleaseListener(GarButtonID button, OnClickListener listener){
	releaseListeners[button] = listener;
}

void gar_reg_OnHoldListener(GarButtonID button_id, uint16_t threshold, uint16_t frequency, OnHoldListener listener){
	holdListeners[button_id].holdListener = listener;
	holdListeners[button_id].holdThreshold = threshold;
	holdListeners[button_id].trigFrequency = frequency;
}

void gar_drop_OnClickListener(GarButtonID button){
	clickListeners[button] = 0;
}

void gar_drop_OnReleaseListener(GarButtonID button){
	releaseListeners[button] = 0;
}

void gar_drop_OnHoldListener(GarButtonID button_id){
	holdListeners[button_id].holdListener = 0;
}

void XBC_NSS_HANDLER(void) {
	if (EXTI_GetITStatus(GAR_NSS_EXTILINE) == SET) {
		spi_gar_update_ssi();
		EXTI_ClearITPendingBit(GAR_NSS_EXTILINE);
	}
}
