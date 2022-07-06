/*!
 * @file pixie_chroma_lite_internal.cpp
 *
 * Designed specifically to work with Pixie Chroma:
 * ----> www.lixielabs.com/chroma
 *
 * Last Updated by Connor Nishijima on 4/30/22
 */

#include "PixieChromaLite.h" 
#include "utility/pixie_utility.h"
#include "platforms/pixie_avr.h"
#include "platforms/pixie_esp8266.h"
#include "platforms/pixie_rp2040.h"

#if defined(ARDUINO_ARCH_RP2040)
	#include <hardware/sync.h>
#endif

// ---------------------------------------------------------------------------------------------------------|
// -- PUBLIC CLASS FUNCTIONS -------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------------------------------|

PixieChromaLite::PixieChromaLite(){}


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %% FUNCTIONS - SETUP %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void PixieChromaLite::begin( const uint8_t data_pin, uint8_t num_pixies ){
	init_pin(data_pin);
	
	num_displays = num_pixies * 2;
	num_columns  = ( num_displays + 1 ) * 7;
	matrix_width = num_displays * 5;

	mask_data  = new uint8_t[ num_columns ];
	color_data = new uint8_t[ num_displays * 3 ];
	
	for(uint16_t i = 0; i < num_displays * 3; i++){
		color_data[i] = 1; // init dark gray
	}
	

	// Here during development
	#if !defined(ATTINY_MODE)
		while (!Serial) {
			delay(1);
		}

		Serial.begin(230400);
		Serial.println("BOOT");
	#endif
}


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %% FUNCTIONS - WRITE %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void PixieChromaLite::add_char( char chr, uint16_t x_pos ){
	if ( chr >= printable_ascii_offset ) {
		chr -= printable_ascii_offset;
	}

	mask_data[x_pos  ] = pgm_read_byte( font + ( chr * 5     ) );
	mask_data[x_pos+1] = pgm_read_byte( font + ( chr * 5 + 1 ) );
	mask_data[x_pos+2] = pgm_read_byte( font + ( chr * 5 + 2 ) );
	mask_data[x_pos+3] = pgm_read_byte( font + ( chr * 5 + 3 ) );
	mask_data[x_pos+4] = pgm_read_byte( font + ( chr * 5 + 4 ) );
}


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %% FUNCTIONS - UPDATING THE MASK / LEDS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void PixieChromaLite::show(){
	#if defined(ARDUINO_ARCH_RP2040)
		uint32_t ints = save_and_disable_interrupts();
	#else
		cli();
	#endif
	
	for(uint8_t disp = 0; disp < num_displays; disp++){
		for(uint8_t y = 0; y < 7; y++){
			for(uint8_t x = 0; x < 5; x++){
				uint16_t mask_column = 7 + ( 7 * disp ) + 1 + x;
				uint8_t mask_bit = bitRead(mask_data[mask_column], y);
				
				if(mask_bit){
					send_pixel(
						color_data[ (3 * disp) + 0 ],
						color_data[ (3 * disp) + 1 ],
						color_data[ (3 * disp) + 2 ]
					);
				}
				else{
					send_pixel(
						0,0,0
					);
				}
			}
		}
	}
	
	#if defined(ARDUINO_ARCH_RP2040)
		restore_interrupts( ints );
	#else
		sei();
	#endif
	
	delayMicroseconds( 50 );
}

#if !defined(ATTINY_MODE)
void PixieChromaLite::print_mask(){
	Serial.println();
	for (uint8_t y = 0; y < 7; y++) {
		for (uint16_t x = 0; x < num_columns; x++) {
			uint8_t column_data = mask_data[x];
			uint8_t mask_val = bitRead(column_data, y);
			int16_t x_temp = x+1;
			if(mask_val){
				Serial.print('#');
			}
			else{
				Serial.print('.');
			}
			Serial.print(' ');
			if(x_temp % 7 == 0 && x_temp >= 0){
				Serial.print(' ');
			}
		}
		Serial.println();
	}
	Serial.println();
}
#endif


void PixieChromaLite::send_byte( uint8_t byte ) {
	for ( uint8_t bit = 0 ; bit < 8 ; bit++ ) {
		send_bit( bitRead( byte , 7 ) );	// Neopixel wants bit in highest-to-lowest order
											// so send highest bit (bit #7 in an 8-bit byte since they start at 0)
		byte <<= 1;                     	// and then shift left so bit 6 moves into 7, 5 moves into 6, etc
	}
}


void PixieChromaLite::send_pixel( uint8_t r, uint8_t g, uint8_t b ) {
  send_byte(g);
  send_byte(r);
  send_byte(b);
}

void PixieChromaLite::set_color( uint8_t r, uint8_t g, uint8_t b ){
	for(uint16_t i = 0; i < num_displays*3; i+=3){
		color_data[i+0] = r;
		color_data[i+1] = g;
		color_data[i+2] = b;
	}
}

void PixieChromaLite::set_color( uint8_t r, uint8_t g, uint8_t b, uint16_t display ){
	color_data[(3*display)+0] = r;
	color_data[(3*display)+1] = g;
	color_data[(3*display)+2] = b;
}

void PixieChromaLite::clear(){
	memset( mask_data, 0, num_columns );
}