#include "garuda_controller.h"
#include "lcd_main.h"

void gar_transmit_tft(void);
void gar_spi_schedule_next(void);

u8 garuda_tft_buf[160] = {' '};

void garuda_tft_init(){
	for (u8 i=0; i<160; i++){
		garuda_tft_buf[i] = ' ';
	}
}

static inline u8 map_char_set(u8 ascii, u8 is_colored){
	if (ascii == ' '){
		return 62 | 0x80;
	}else if(ascii == '-'){
		return (62 | 0xC0);
	}else if(ascii == '<'){
		return (63 | 0x80);
	}else if(ascii == '>'){
		return (63 | 0xC0);
	}
	
	if (is_colored){
		if (ascii >= '0' && ascii <= '9'){
			return (ascii-48)|0xC0;
		}else if (ascii >= 'A' && ascii <= 'Z'){
			return (ascii-55)|0xC0;
		}else if(ascii >= 'a' && ascii <= 'z'){
			return (ascii-61)|0xC0;
		}else if(ascii == '-'){
			return (63|0xC0);
		}
	}else{
		if (ascii >= '0' && ascii <= '9'){
			return (ascii-48)|0x80;
		}else if (ascii >= 'A' && ascii <= 'Z'){
			return (ascii-55)|0x80;
		}else if(ascii >= 'a' && ascii <= 'z'){
			return (ascii-61)|0x80;
		}else if(ascii == '-'){
			return (63|0x80);
		}
	}
	return 62 | 0x80;
}

//First bit 0-command 1-data
void gar_tft_update(){
	GAR_CONNECTION_MODE connection = gar_get_connection();
	
	u16 i = 0;
	for (u8 x=0; x<char_max_x; x++){
		for (u8 y=0; y<char_max_y; y++){
			//x first then y, possibly lower cache miss
			garuda_tft_buf[i++] = map_char_set(tft_get_buffer_char(x, y), tft_get_buffer_color(x, y) != WHITE);
		}
	}
	
	gar_transmit_tft();
}
