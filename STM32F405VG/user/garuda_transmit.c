#include "garuda_controller.h"
#include "lcd_main.h"

static GarPriority garuda_priority;

extern u8 garuda_tft_buf[160];
static u8 spi_tft_pointer = 0;

static u8 spi_pending_command_byte = 0;
static bool spi_command_interrupt = false;

//0-not sending 1-sending uart 2-sending spi
u8 tft_tx_selection = 0;

// 00000XXX - TFT SOF
// 00111111 - TFT EOF
// 0101XXXX - Buzzer
// 0110XXXX - LED A
// 0111XXXX - LED B

#define TFT_SOF 				0x00
#define TFT_EOF 				0x3F
#define BUZZER_BASE			0x50
#define LED_BASE				0x60

void garuda_transmit_init(GarPriority priority){
	garuda_priority = priority;
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//DMA init for uart tft tx
	DMA_InitTypeDef DMA_InitStructure;  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(UARTPorts[BT_GAR_COM].uart->DR));
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)(&garuda_tft_buf[0]);
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = 160;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream6, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Stream6, DISABLE);
	DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	SPI_I2S_SendData(SPI_GAR_PORT, TFT_SOF);
}

void gar_spi_schedule_next(){
	
	if (spi_command_interrupt){
		SPI_I2S_SendData(SPI_GAR_PORT, spi_pending_command_byte);
		spi_command_interrupt = false;
		
	}else if (tft_tx_selection == 2){
		if (spi_tft_pointer == 0){
			SPI_I2S_SendData(SPI_GAR_PORT, TFT_SOF + tft_get_orientation());
			spi_tft_pointer++;
			
		} else if ((spi_tft_pointer-1) < 160){
			SPI_I2S_SendData(SPI_GAR_PORT, garuda_tft_buf[spi_tft_pointer-1]);
			spi_tft_pointer++;
			
		} else if (spi_tft_pointer-1 == 160){
			SPI_I2S_SendData(SPI_GAR_PORT, TFT_EOF);
			spi_tft_pointer++;
			
		} else{
			spi_tft_pointer = 0;
			tft_tx_selection = 0;
			SPI_I2S_SendData(SPI_GAR_PORT, TFT_SOF);
		}
	}
}

void gar_transmit_tft(){
	GAR_CONNECTION_MODE connection = gar_get_connection();
	
	if (garuda_priority == GAR_SPI_FIRST){
		if (connection == GAR_BOTH_CONNECTED || connection == GAR_ONLY_SPI_CONNECTED){
			
			if (tft_tx_selection != 2){
				spi_tft_pointer = 0;
			}
			tft_tx_selection = 2;
		}else if (connection == GAR_ONLY_BT_CONNECTED){
			if (tft_tx_selection != 1){
				tft_tx_selection = 1;
				uart_tx_byte(BT_GAR_COM, TFT_SOF + tft_get_orientation());
				
				DMA_Cmd(DMA1_Stream6, ENABLE);
				USART_DMACmd(UARTPorts[BT_GAR_COM].uart, USART_DMAReq_Tx, ENABLE);
			}
		}
	}else{
		if (connection == GAR_BOTH_CONNECTED || connection == GAR_ONLY_BT_CONNECTED){
			if (tft_tx_selection != 1){
				tft_tx_selection = 1;
				uart_tx_byte(BT_GAR_COM, TFT_SOF + tft_get_orientation());
				
				DMA_Cmd(DMA1_Stream6, ENABLE);
				USART_DMACmd(UARTPorts[BT_GAR_COM].uart, USART_DMAReq_Tx, ENABLE);
			}
		}else if (connection == GAR_ONLY_SPI_CONNECTED){
			if (tft_tx_selection != 2){
				spi_tft_pointer = 0;
			}
			tft_tx_selection = 2;
		}
	}
}

void DMA1_Stream6_IRQHandler(){
	if(DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6) != RESET){
		DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
		DMA_Cmd(DMA1_Stream6, DISABLE);
		USART_DMACmd(UARTPorts[BT_GAR_COM].uart, USART_DMAReq_Tx, DISABLE);
		uart_tx_byte(BT_GAR_COM, TFT_EOF);
		tft_tx_selection = 0;
	}
}



// SPI disabled for command because there are too many shitty noises

//Buzzer control
void gar_buzzer_off(void){
	u8 command = BUZZER_BASE | 0x00;;
	//spi_command_interrupt = true;
	//spi_pending_command_byte = command;
	uart_tx_byte(BT_GAR_COM, command);
}

void gar_buzzer_on(void){
	u8 command = BUZZER_BASE | 0x05;
	//spi_command_interrupt = true;
	//spi_pending_command_byte = command;
	uart_tx_byte(BT_GAR_COM, command);
}

void gar_buzzer_short_beep(void){
	u8 command = BUZZER_BASE | 0x0A;
	//spi_command_interrupt = true;
	//spi_pending_command_byte = command;
	uart_tx_byte(BT_GAR_COM, command);
}

void gar_buzzer_long_beep(void){
	u8 command = BUZZER_BASE | 0x0F;
	//spi_command_interrupt = true;
	//spi_pending_command_byte = command;
	uart_tx_byte(BT_GAR_COM, command);
}

//LED control
void gar_led_off(GarLed led){
	u8 command = LED_BASE | (led << 4) | 0x00;
	//spi_command_interrupt = true;
	//spi_pending_command_byte = command;
	uart_tx_byte(BT_GAR_COM, command);
}

void gar_led_on(GarLed led){
	u8 command = LED_BASE | (led << 4) | 0x05;
	//spi_command_interrupt = true;
	//spi_pending_command_byte = command;
	uart_tx_byte(BT_GAR_COM, command);
}

void gar_led_rapid_flash(GarLed led){
	u8 command = LED_BASE | (led << 4) | 0x0A;
	//spi_command_interrupt = true;
	//spi_pending_command_byte = command;
	uart_tx_byte(BT_GAR_COM, command);
}

void gar_led_slow_flash(GarLed led){
	u8 command = LED_BASE | (led << 4) | 0x0F;
	//spi_command_interrupt = true;
	//spi_pending_command_byte = command;
	uart_tx_byte(BT_GAR_COM, command);
}

