/*!
 * @file pixie_utility.h
 *
 * Contains utility arrays/functions for internal use. 
 *
 * Last Updated by Connor Nishijima on 4/19/22
 */

#ifndef pixie_utility_h
#define pixie_utility_h

// Extra utilities for Pixie_Chroma_Lite code

/*!
@brief
Homebrew C function to convert double precision floats to char*. ([Arduino Forum link](https://forum.arduino.cc/t/ftoa/63464))
@param   input      Double value to convert
@param   buffer     char* to store resulting string in
@param   precision  How many places after the decimal point will be converted
@return  A char array equivalent of the input double
*/
char* dtoa(double input, char* buffer, int precision) {
	int32_t whole_part = (int32_t) input;

	// Deposit the whole part of the number.
	itoa( whole_part, buffer, 10 );

	// Now work on the faction if we need one.
	if( precision > 0 ) {

		// We do, so locate the end of the string and insert
		// a decimal point.
		char* end_of_string = buffer;
		while( *end_of_string != '\0' ){
			end_of_string++;
        }
		*end_of_string++ = '.'; 

		// Now work on the fraction, being sure to turn any negative
		// values positive.
		if (input < 0) {
			input *= -1;
			whole_part *= -1;
		}
		
		double fraction = input - whole_part;
		while (precision > 0) {
			// Multiply by ten and pull out the digit.
			fraction *= 10;
			whole_part = (int32_t) fraction;
			*end_of_string++ = '0' + whole_part;

			// Update the fraction and move on to the
			// next digit.
			fraction -= whole_part;
			precision--;
		}

		// Terminate the string.
		*end_of_string = '\0';
	}
	
    return buffer;
}

#endif
