# Pixie Chroma Lite

Pixie Chroma, but way less RAM! ***(Like, 98% less!)***

Let's say you have 20 Pixie Chroma boards, but you want to run them off of an Arduino Uno.

That's (70 * 20) = **1,400 LEDs**.

Even worse, that's (1,400 * 3) = **4,200 bytes** of RAM needed to store the RGB data!

**How about reducing color resolution to single displays, dropping some of the more advanced features, and getting that exact scenario to run with just **299 bytes** (14%) of dynamic memory on an Arduino Uno?**

The biggest hurdle is that we can't store the display buffer whole, we have to take *notes* about what we'll see like text and color, and render it live to the LEDs one-at-a-time before completely losing it from our memory!

This still achieves around ~60FPS on a 4 Pixie (280 LEDs) display on a 16MHz micro, but without fancy animation anything above 10 FPS is fine for most projects. (Can you read unique text messages at greater than 10FPS anyways?)

Tested / working so far:

- ATMEGA168
- ATMEGA328P
- ATMEGA16U2
- ATMEGA32U4
- ATMEGA1280
- ATMEGA2560
- ATTINY45
- ATTINY85 // ATTINY\*4 and 0-series not yet tested

Not working yet / planned:

- ~ATTINY series~
- SAMD21
- ESP8266
- ESP32
- Teensy series
