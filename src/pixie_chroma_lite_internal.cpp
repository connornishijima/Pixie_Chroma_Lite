/*!
 * @file pixie_chroma_lite_internal.cpp
 *
 * Designed specifically to work with Pixie Chroma:
 * ----> https://connornishijima.github.io/PixieChroma
 *
 * Last Updated by Connor Nishijima on 4/19/22
 */

#include "Pixie_Chroma_Lite.h" 
#include "utility/pixie_utility.h"

// TEMPORARY PIN SETTINGS ###############################################################

#define PIXEL_PORT  PORTB  // Port of the pin the pixels are connected to
#define PIXEL_DDR   DDRB   // Port of the pin the pixels are connected to
#define PIXEL_BIT   4      // Bit of the pin the pixels are connected to

// TEMPORARY PIN SETTINGS ###############################################################


// These are the timing constraints taken mostly from the WS2812 datasheets 
// These are chosen to be conservative and avoid problems rather than for maximum throughput 

#define T1H  900    // Width of a 1 bit in ns
#define T1L  600    // Width of a 1 bit in ns

#define T0H  400    // Width of a 0 bit in ns
#define T0L  900    // Width of a 0 bit in ns

#define NS_PER_SEC (1000000000L)          // Note that this has to be SIGNED since we want to be able to check for negative values of derivatives

#define CYCLES_PER_SEC (F_CPU)

#define NS_PER_CYCLE ( NS_PER_SEC / CYCLES_PER_SEC )
#define NS_TO_CYCLES(n) ( (n) / NS_PER_CYCLE )

// ---------------------------------------------------------------------------------------------------------|
// -- PUBLIC CLASS FUNCTIONS -------------------------------------------------------------------------------|
// ---------------------------------------------------------------------------------------------------------|

Pixie_Chroma_Lite::Pixie_Chroma_Lite(){}


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %% FUNCTIONS - SETUP %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Pixie_Chroma_Lite::begin( const uint8_t data_pin, uint8_t num_pixies ){
	bitSet( PIXEL_DDR , PIXEL_BIT );
	
	num_displays = num_pixies * 2;
	num_columns = ( num_displays + 2 ) * 7;
	matrix_width = num_displays * 5;

	mask_data  = new uint8_t[ num_columns ];
	color_data = new uint8_t[ num_displays * 3 ];
	
	for(uint16_t i = 0; i < num_displays * 3; i++){
		color_data[i] = 1; // init dark gray
	}
	

	while (!Serial) {
		delay(1);
	}

	Serial.begin(230400);
	Serial.println("BOOT");
}


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %% FUNCTIONS - WRITE %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Pixie_Chroma_Lite::add_char( char chr, uint16_t x_pos ){
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

void Pixie_Chroma_Lite::show(){
	cli();
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
	sei();
	
	delayMicroseconds( 250 );
}


void Pixie_Chroma_Lite::print_mask(){
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


void Pixie_Chroma_Lite::send_bit( bool bit_val ) {
  if ( bit_val ) { // 0 bit
    asm volatile (
      "sbi %[port], %[bit] \n\t"        // Set the output bit
      ".rept %[on_cycles] \n\t"                                // Execute NOPs to delay exactly the specified number of cycles
      "nop \n\t"
      ".endr \n\t"
      "cbi %[port], %[bit] \n\t"                              // Clear the output bit
      ".rept %[off_cycles] \n\t"                               // Execute NOPs to delay exactly the specified number of cycles
      "nop \n\t"
      ".endr \n\t"
      ::
      [port]       "I" (_SFR_IO_ADDR(PIXEL_PORT)),
      [bit]        "I" (PIXEL_BIT),
      [on_cycles]  "I" (NS_TO_CYCLES(T1H) - 2),    // 1-bit width less overhead  for the actual bit setting, note that this delay could be longer and everything would still work
      [off_cycles] "I" (NS_TO_CYCLES(T1L) - 2)     // Minimum interbit delay. Note that we probably don't need this at all since the loop overhead will be enough, but here for correctness

    );
  }
  else{ // 1 bit

    // **************************************************************************
    // This line is really the only tight goldilocks timing in the whole program!
    // **************************************************************************

    asm volatile (
      "sbi %[port], %[bit] \n\t"      // Set the output bit
      ".rept %[on_cycles] \n\t"        // Now timing actually matters. The 0-bit must be long enough to be detected but not too long or it will be a 1-bit
      "nop \n\t"                                              // Execute NOPs to delay exactly the specified number of cycles
      ".endr \n\t"
      "cbi %[port], %[bit] \n\t"                              // Clear the output bit
      ".rept %[off_cycles] \n\t"                               // Execute NOPs to delay exactly the specified number of cycles
      "nop \n\t"
      ".endr \n\t"
      ::
      [port]       "I" (_SFR_IO_ADDR(PIXEL_PORT)),
      [bit]        "I" (PIXEL_BIT),
      [on_cycles]  "I" (NS_TO_CYCLES(T0H) - 2),
      [off_cycles] "I" (NS_TO_CYCLES(T0L) - 2)
    );
  }  
}


void Pixie_Chroma_Lite::send_byte( uint8_t byte ) {
	for ( uint8_t bit = 0 ; bit < 8 ; bit++ ) {
		send_bit( bitRead( byte , 7 ) );	// Neopixel wants bit in highest-to-lowest order
											// so send highest bit (bit #7 in an 8-bit byte since they start at 0)
		byte <<= 1;                     	// and then shift left so bit 6 moves into 7, 5 moves into 6, etc
	}
}


void Pixie_Chroma_Lite::send_pixel( uint8_t r, uint8_t g, uint8_t b ) {
  send_byte(g);
  send_byte(r);
  send_byte(b);
}

void Pixie_Chroma_Lite::set_color( uint8_t r, uint8_t g, uint8_t b ){
	for(uint16_t i = 0; i < num_displays*3; i+=3){
		color_data[i+0] = r;
		color_data[i+1] = g;
		color_data[i+2] = b;
	}
}

void Pixie_Chroma_Lite::set_color( uint8_t r, uint8_t g, uint8_t b, uint16_t display ){
	color_data[(3*display)+0] = r;
	color_data[(3*display)+1] = g;
	color_data[(3*display)+2] = b;
}

void Pixie_Chroma_Lite::clear(){
	memset( mask_data, 0, num_columns );
}