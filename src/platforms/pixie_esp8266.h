// TODO: Add header text to pixie_esp8266.h

#if defined(ESP8266)
#include "Arduino.h"

bool fast_mode;
uint8_t _data_pin;

void init_pin(uint8_t data_pin){
	_data_pin = data_pin;
	pinMode(_data_pin, OUTPUT);
	
	if(F_CPU == 160000000){
		fast_mode = true;
	}
	else if(F_CPU == 80000000){
		fast_mode = false;
	}
}

void IRAM_ATTR send_bit( bool bit_val ) {
	if(fast_mode){
		if ( bit_val ) { // 1 bit
			GPOS = (1 << _data_pin);

			asm("nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop;");

			GPOC = (1 << _data_pin);

			asm("nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop;");
		}
		else { // 0 bit
			GPOS = (1 << _data_pin);

			asm("nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop;");
			GPOC = (1 << _data_pin);

			asm("nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop;");
		}
	}
	
	else{ // Slower 80MHz speed
		if ( bit_val ) {
			GPOS = (1 << _data_pin);

			asm("nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop;");

			GPOC = (1 << _data_pin);

			asm("nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop;");
		}
		else {
			GPOS = (1 << _data_pin);

			asm("nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop;");
				
			GPOC = (1 << _data_pin);

			asm("nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop; nop; nop;"
				"nop; nop; nop; nop; nop; nop;");
		}
	}
}

#endif