// TODO: Add header text to pixie_rp2040.h
// Only 133MHz supported here
// 7.5nS per CPU cycle @ 133MHz

#if defined(ARDUINO_ARCH_RP2040)
#include "Arduino.h"

uint8_t _data_pin;

void init_pin(uint8_t data_pin){ // Parameter not used yet
	_data_pin = data_pin;
	pinMode( _data_pin, OUTPUT );
}

void send_bit( bool bit_val ) {
  if ( bit_val ) { // 1 bit	
    gpio_put(_data_pin, 1);
	asm volatile (
  	     "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; "
		);

	gpio_put(_data_pin, 0);
	asm volatile (
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		);

  }
  else{ // 0 bit
	gpio_put(_data_pin, 1);
	asm volatile (
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; "
		);

	gpio_put(_data_pin, 0);
	asm volatile (
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; "
		 "nop; nop; nop; nop; nop; nop; "
		);
  }
}



#endif