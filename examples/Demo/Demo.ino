#include "PixieChromaLite.h"
PixieChromaLite pix;

// WARNING!
//
// Only ATMega and ATTiny*5 series is tested thus far!
//
// You must hard-code your DATA PIN and PORT
// in "platforms/pixie_avr.h" under
// "TEMPORARY PIN SETTINGS". This will be
// simplified in the future.
//
// Default pin is PB4 for now. That's:
//
// Pin 12 on Uno         (ATMEGA328P)
// Pin 10 on Mega        (ATMEGA2560)
// Pin 8  on Leonardo    (ATMEGA32u4)
// Pin 4  on Trinket     (ATTINY45/85)

#define NUM_PIXIES 12 // Number of Pixie Chroma PCBs
#define DATA_PIN 0    // NOT YET USED. SEE COMMENT UP TOP

uint8_t color_index = 2;
uint8_t colors[3][3] = {
  {255,255,0},
  {0,255,255},
  {255,0,255}
};

void setup() {
  pix.begin(DATA_PIN, NUM_PIXIES);
}

void loop() {
  static int16_t lev = 0;
  static int16_t dir = -2;
  
  pix.set_color(
    (colors[color_index][0] * lev) >> 8,
    (colors[color_index][1] * lev) >> 8,
    (colors[color_index][2] * lev) >> 8
  );
  pix.show();

  lev += dir;
  if(dir == 2 && lev >= 25){
    lev = 25;
    dir = -2;
  }
  else if(dir == -2 && lev <= 0){
    lev = 0;
    dir = 2;

    color_index++;
    if(color_index >= 3){
      color_index = 0;
    }
    
    pix.clear();
    if(color_index == 0){
      print_string("Hello, from an ATtiny85!");
    }
    else if(color_index == 1){
      print_string("Rendered directly to the");
    }
    else if(color_index == 2){
      print_string("LEDs, matrix not in RAM!");
    }
  }
}

void print_string(char* message){
  uint16_t len = strlen(message);
  for(uint16_t i = 0; i < len; i++){
    pix.add_char(message[i], 8+(7*i));
  }
}
