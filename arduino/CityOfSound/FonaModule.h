///////////////////////////////////////////////////////////////////
// A Fona interface for receiving SMS
//
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2016, Imanol Gomez
///////////////////////////////////////////////////////////////////

#pragma once
#include "Arduino.h"
#include "Adafruit_FONA.h"

// We default to using software serial. If you want to use hardware serial
// (because softserial isnt supported) comment out the following three lines 
// and uncomment the HardwareSerial line
#include <SoftwareSerial.h>


#define FONA_RX  9
#define FONA_TX  8
#define FONA_RST 4
#define FONA_RI  7




class FonaModule{
public:
  /// set all information necessary to use the module and initialize it
  void setup();

  void initializeFona();
  void deleteAllSMS();
  void startFona();
  
  void update();       ///< checks for sms notifications and gsm connectivity in a non-blocking way.

  bool isNewMessage() {return newMessage;}
  const String& getCurrentMessage(){return currentMessage;}

  char fonaInBuffer[64];
  char smsbuffer[255];  // this is a large buffer for sms

  bool readBattPercent();

  bool isAvailable();

 SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
 SoftwareSerial *fonaSerial   = &fonaSS;
  
private:

    Adafruit_FONA* fona;  //Class controlling the GSM module
    bool newMessage;
    String currentMessage;
    bool lipoOn;
    
};


void FonaModule::setup()
{
    initializeFona();
    deleteAllSMS();
}

void FonaModule::startFona()
{
   // make it slow so its easy to read!
    fonaSerial->begin(4800);
    if (! fona->begin(*fonaSerial)) {
      Serial.println(F("Couldn't find FONA"));
      while(1);
    }
}


void FonaModule::initializeFona()
{
    fona = new Adafruit_FONA(FONA_RST);
  
    Serial.println(F("FONA SMS caller ID test"));
    Serial.println(F("Initializing....(May take 3 seconds)"));

    startFona();
    
//    while(!readBattPercent())
//    {
//      Serial.println(F("Attemtping to start FONA"));
//      startFona();
//    }
//    

    
    
    Serial.println(F("FONA is OK"));
    
    // Print SIM card IMEI number.
    char imei[15] = {0}; // MUST use a 16 character buffer for IMEI!
    uint8_t imeiLen = fona->getIMEI(imei);
    if (imeiLen > 0) {
      Serial.print("SIM card IMEI: "); Serial.println(imei);
    }
    
    Serial.println("FONA Ready");
}


void FonaModule::deleteAllSMS()
{
    if(fona->deleteAllSMS(4))
    {
      Serial.println("Deleting All SMS ");
    }
}

bool FonaModule::readBattPercent()
{
   // read the battery voltage and percentage
        uint16_t vbat;
        
   if (! fona->getBattPercent(&vbat)) {
          Serial.println(F("Failed to read Batt"));
          return false;
        } else {
          Serial.print(F("VPct = ")); Serial.print(vbat); Serial.println(F("%"));
          return true;
        }
}

 void FonaModule::update()
 {
     newMessage = false;
      //this->readBattPercent();
     if (fona->available())      //any data available from the FONA?
     {
        // this is a large buffer for sms
       
          char* bufPtr = fonaInBuffer;    //handy buffer pointer

          int slot = 0;            //this will be the slot number of the SMS
          int charCount = 0;
          //Read the notification into fonaInBuffer
           do  
           {
              *bufPtr = fona->read();
              //Serial.write(*bufPtr);
              delay(1);
            } while ((*bufPtr++ != '\n') && (fona->available()) && (++charCount < (sizeof(fonaInBuffer)-1)));
    
          //Add a terminal NULL to the notification string
           *bufPtr = 0;
      
         //Scan the notification string for an SMS received notification.
          //  If it's an SMS message, we'll get the slot number in 'slot'
         if (1 == sscanf(fonaInBuffer, "+CMTI: \"SM\",%d", &slot)) 
         {
             // Retrieve SMS value.
              uint16_t smslen;
            if (! fona->readSMS(slot, smsbuffer, 250, &smslen)) { // pass in buffer and max len!
              Serial.println("Failed!");
              deleteAllSMS();
            }
            else{
              Serial.print(F("***** SMS #")); Serial.print(slot);
              Serial.print(" ("); Serial.print(smslen); Serial.println(F(") bytes *****"));
              Serial.println(smsbuffer);
              Serial.println(F("*****"));
              newMessage = true;
              currentMessage = String(smsbuffer);
            }
      
            if (fona->deleteSMS(slot)) { // delete the original msg after it is processed, otherwise
              Serial.println(F("Deleted Message"));
            } else {
              Serial.println(F("Couldn't delete"));
              deleteAllSMS();
            }
          }
     }
  
 }















