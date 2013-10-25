// Trinket/Gemma + LED matrix backpack jewelry.  Plays animated
// sequence on LED matrix.  Press reset button to display again,
// or add optional momentary button between pin #1 and +V.
// THERE IS NO ANIMATION DATA IN THIS SOURCE FILE, you should
// rarely need to change anything here.  EDIT anim.h INSTEAD.
 
#define BRIGHTNESS    7 // 0=min, 15=max
#define I2C_ADDR   0x70 // Edit if backpack A0/A1 jumpers set
 
#include <TinyWireM.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include "anim.h"       // Animation data is located here
 
void ledCmd(uint8_t x) { // Issue command to LED backback driver
  TinyWireM.beginTransmission(I2C_ADDR);
  TinyWireM.write(x);
  TinyWireM.endTransmission();
}
 
void clear(void) { // Clear display buffer
  TinyWireM.beginTransmission(I2C_ADDR);
  for(uint8_t i=0; i<17; i++) TinyWireM.write(0);
  TinyWireM.endTransmission();
}
 
void setup() {
  power_timer1_disable();    // Disable unused peripherals
  power_adc_disable();       // to save power
  PCMSK |= _BV(PCINT1);      // Set change mask for pin 1
  TinyWireM.begin();         // I2C init
  clear();                   // Blank display
  ledCmd(0x21);              // Turn on oscillator
  ledCmd(0xE0 | BRIGHTNESS); // Set brightness
  ledCmd(0x81);              // Display on, no blink
}
 
//uint8_t rep = REPS;
 
void loop() {
 
  for(int i=0; i<sizeof(anim); i) { // For each frame...
    TinyWireM.beginTransmission(I2C_ADDR);
    TinyWireM.write(0);             // Start address
    for(uint8_t j=0; j<8; j++) {    // 8 rows...
      TinyWireM.write((pgm_read_byte(&anim[i++]) * 0x8080) >> 8); // ROL
      TinyWireM.write(0);
    }
    TinyWireM.endTransmission();
    delay(pgm_read_byte(&anim[i++]) * 10);
  }
}
 
//ISR(PCINT0_vect) {} // Button tap
