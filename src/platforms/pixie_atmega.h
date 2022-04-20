#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega16u2__) || defined(__AVR_ATmega32u4__)

#include "Arduino.h"

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

void init_pin(){
	bitSet( PIXEL_DDR , PIXEL_BIT );
}

void send_bit( bool bit_val ) {
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

#endif