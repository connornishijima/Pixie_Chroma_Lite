/*! 
 * @file Pixie_Chroma_Lite.h 
 *
 * Designed specifically to work with Pixie Chroma:
 * ----> https://connornishijima.github.io/PixieChroma
 *
 * Last Updated by Connor Nishijima on 4/19/22
 */

/*

     ██  ██  ██  ██          ██  ██  ██      ██              ██      ██  ██  ██      ██  ██  ██  ██  ██
  
     ██              ██          ██          ██              ██          ██          ██  
  
     ██              ██          ██              ██      ██              ██          ██  
  
     ██  ██  ██  ██              ██                 ██                   ██          ██  ██  ██  ██  ██
   
     ██                          ██             ██      ██               ██          ██  
  
     ██                          ██          ██              ██          ██          ██  
  
     ██                      ██  ██  ██      ██              ██      ██  ██  ██      ██  ██  ██  ██  ██
  
                              ██████ ██   ██ ██████   ██████  ███    ███  █████  
                             ██      ██   ██ ██   ██ ██    ██ ████  ████ ██   ██ 
                             ██      ███████ ██████  ██    ██ ██ ████ ██ ███████ 
                             ██      ██   ██ ██   ██ ██    ██ ██  ██  ██ ██   ██ 
                              ██████ ██   ██ ██   ██  ██████  ██      ██ ██   ██
   
													(LITE)
   
                                                by @LixieLabs

    ----------------------------------------------------------------------------------------------------

    Welcome to the source code of the Pixie Chroma Lite library!

    "Pixie_Chroma_Lite.h" is the main file, which loads all others, including
    the pixie_chroma_lite_internal.h/.cpp that contain the Pixie_Chroma_Lite class definitions.

	The custom SK6805 driver is derived from:
	https://wp.josh.com/2014/05/13/ws2812-neopixels-are-not-so-finicky-once-you-get-to-know-them/

    ----------------------------------------------------------------------------------------------------
*/

#include "Arduino.h" // Needed for things like uint8_t typedefs

#include "utility/pixie_font.h" // ----------- ASCII font
#include "pixie_chroma_lite_internal.h" // --- Main library code