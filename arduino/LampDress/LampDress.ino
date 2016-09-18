/***************************************************
  This sketch shows how to remote control wearable LEDs using text messages.

  I've used the Adafruit Feather FONA. It has  with GSM capabilities and is able to receive and decode SMS.

  A chain of individually adressable WS2801 LEDs is controlled by the Adafruit Feather and the Adafruit WS2801 library.

  This code is under A Creative Commons Attribution/Share-Alike License http://creativecommons.org/licenses/by-sa/4.0/
   (2016) by Imanol Gomez

/****************************************************


 ///////////////////////How to wire things up:///////////////////////

 
-----------------Connection to the LEDs:--------------------------

- We use 12mm Diffused Thin Digital RGB LED Pixels (Strand of 25) - WS2801.
- The pixels are connected by a 4-conductor cable. +5V (Red), Ground (Blue), Data (Yellow) and Clock (Green). 
- At 5 volts, they draw a maximum of 60 milliamps per pixel: 20 mA each for red, green and blue..
- 5V/2A supply should be able to drive 2 or more strands

// LED strand GND/0V/-      <----> Adafruit Feather GND 
// LED strand 5V/+          <---->  Adafruit Feather USB output  
// LED strand MOSI/Data in  <----> Adafruit Feather SPI MOSI (GPIO #13)
// LED strand SCLK/Clock    <----> Adafruit Feather SPI SCK  (GPIO #14) 
*/


#include "FonaModule.h"
#include "LedsManager.h"
#include "ReceiveHandler.h"


FonaModule fonaModule;
LedsManager leds;
ReceiveHandler receiver(&leds);


void setup() {

  fonaModule.setup();
  leds.setup();
  receiver.setup();
  
}


void loop() {

  leds.update();
  
  fonaModule.update();
    
  if(fonaModule.isNewMessage()){
      receiver.parseMessage(fonaModule.getCurrentMessage());
  }

}
