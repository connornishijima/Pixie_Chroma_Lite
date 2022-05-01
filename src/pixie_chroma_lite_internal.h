/*! 
 * @file pixie_chroma_lite_internal.h
 *
 * Designed specifically to work with Pixie Chroma:
 * ----> https://connornishijima.github.io/PixieChroma
 *
 * Last Updated by Connor Nishijima on 4/19/22
 */

#ifndef pixie_chroma_lite_h
#define pixie_chroma_lite_h

#include "Arduino.h"

/*! ############################################################################
    @brief
    This is the software documentation for using Pixie Chroma Lite functions on
    Arduino! **For full example usage, see File > Examples > Pixie_Chroma_Lite
	inside Arduino!**.
*///............................................................................
class Pixie_Chroma_Lite{
	public:
		/** @brief Construct a Pixie_Chroma_Lite class object */
		Pixie_Chroma_Lite(); 
		
		/*+-- Functions - Setup ------------------------------------------------------------*/ 
		/*|*/ void begin( const uint8_t data_pin, uint8_t num_pixies );
		/*+-- Functions - print(  ) --------------------------------------------------------*/ 
		/*|*/ void add_char( char chr, uint16_t x_pos );
		/*+-- Functions - Updating the mask/LEDs -------------------------------------------*/
		/*|*/ void clear();
		/*|*/ void show();
		/*+-- Functions - Debug ------------------------------------------------------------*/
		#if !defined(ATTINY_MODE)
		/*|*/ void print_mask();
		#endif
		/*+---------------------------------------------------------------------------------*/
		void set_color( uint8_t r, uint8_t g, uint8_t b );
		void set_color( uint8_t r, uint8_t g, uint8_t b, uint16_t display );

		// Variables -------------------------------------------------------------------------

		uint8_t  num_displays;
		uint16_t num_columns;
		uint16_t matrix_width;
		uint8_t *mask_data;
		uint8_t *color_data;

	private:
		// Functions ----------------------------------
		
		void send_byte( uint8_t byte );
		void send_pixel( uint8_t r, uint8_t g, uint8_t b );
		
	};

#endif
