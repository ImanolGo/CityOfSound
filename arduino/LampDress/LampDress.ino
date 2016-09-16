/***************************************************
  This sketch shows how to remote control wearable LEDs using text messages.

  We've used the Adafruit Feather FONA. It has  with GSM capabilities and is able to receive and decode SMS.

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

 /***************************************************
  This sketch shows how to remote control wearable LEDs using text messages.

  We've used the Adafruit Feather FONA. It has  with GSM capabilities and is able to receive and decode SMS.

  A chain of individually adressable WS2801 LEDs is controlled by the Adafruit Feather and the Adafruit WS2801 library.

  This code is under A Creative Commons Attribution/Share-Alike License http://creativecommons.org/licenses/by-sa/4.0/
   (2016) by Imanol Gomez

/****************************************************

*/

#include "FonaModule.h"



// this is a large buffer for replies
char replybuffer[255];

// We default to using software serial. If you want to use hardware serial
// (because softserial isnt supported) comment out the following three lines 
// and uncomment the HardwareSerial line
#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

// Hardware serial is also possible!
//  HardwareSerial *fonaSerial = &Serial1;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

void setup() {
  while (!Serial);

  Serial.begin(115200);
  Serial.println(F("FONA SMS caller ID test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  // make it slow so its easy to read!
  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while(1);
  }
  Serial.println(F("FONA is OK"));

  // Print SIM card IMEI number.
  char imei[15] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("SIM card IMEI: "); Serial.println(imei);
  }
  
  Serial.println("FONA Ready");
}

  
char fonaInBuffer[64];          //for notifications from the FONA

void loop() {
  
  char* bufPtr = fonaInBuffer;    //handy buffer pointer
  
  if (fona.available())      //any data available from the FONA?
  {
    int slot = 0;            //this will be the slot number of the SMS
    int charCount = 0;
    //Read the notification into fonaInBuffer
    do  {
      *bufPtr = fona.read();
      Serial.write(*bufPtr);
      delay(1);
    } while ((*bufPtr++ != '\n') && (fona.available()) && (++charCount < (sizeof(fonaInBuffer)-1)));
    
    //Add a terminal NULL to the notification string
    *bufPtr = 0;
    
    //Scan the notification string for an SMS received notification.
    //  If it's an SMS message, we'll get the slot number in 'slot'
    if (1 == sscanf(fonaInBuffer, "+CMTI: \"SM\",%d", &slot)) {
      Serial.print("slot: "); Serial.println(slot);
      
      char callerIDbuffer[32];  //we'll store the SMS sender number in here
      
      // Retrieve SMS sender address/phone number.
      if (! fona.getSMSSender(slot, callerIDbuffer, 31)) {
        Serial.println("Didn't find SMS message in slot!");
      }
      Serial.print(F("FROM: ")); Serial.println(callerIDbuffer);
      
      //Send back an automatic response
      Serial.println("Sending reponse...");
      if (!fona.sendSMS(callerIDbuffer, "Hey, I got your text!")) {
        Serial.println(F("Failed"));
      } else {
        Serial.println(F("Sent!"));
      }
      
      // delete the original msg after it is processed
      //   otherwise, we will fill up all the slots
      //   and then we won't be able to receive SMS anymore
      if (fona.deleteSMS(slot)) {
        Serial.println(F("OK!"));
      } else {
        Serial.println(F("Couldn't delete"));
      }
    }
  }
}
