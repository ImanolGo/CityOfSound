///////////////////////////////////////////////////////////////////
// Class managing and parsing the incoming messages
//
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2016, Imanol Gomez
///////////////////////////////////////////////////////////////////

#pragma once
#include "Arduino.h"
#include "LedsManager.h"
#include "Utils.h"

class ReceiveHandler{
private:
  LedsManager* ledsManager;  ///< it sends the commands to the leds manager
public:
  ReceiveHandler(LedsManager* ledsManager);
  void setup() {}
  void parseMessage(String message);
  void setCommands(const String& color, const String& pattern);
  void setColor(const String& color);
  void setPattern(const String& pattern);
};


ReceiveHandler::ReceiveHandler(LedsManager* ledsManager)
{
  this->ledsManager=ledsManager;
};

void ReceiveHandler::parseMessage(String message)
{
  int hashTagIndex = message.indexOf('#');
  //  Search for the next comma just after the first
  int secondHashTagIndex = message.indexOf('#', hashTagIndex+1);

  String firstString = message.substring(0, hashTagIndex);
  String scondString = message.substring(hashTagIndex+1, secondHashTagIndex);
  String thirdString = message.substring(secondHashTagIndex+1); // To the end of the string

  scondString = utf8ascii(scondString);
  thirdString = utf8ascii(thirdString);

  setCommands(scondString, thirdString);
  
}

void ReceiveHandler::setCommands(const String& color, const String& pattern)
{
      Serial.print("Color: "); Serial.println(color);
      Serial.print("Pattern: "); Serial.println(pattern);
      
      this->setColor(color);
      this->ledsManager->nextPattern();
      this->setPattern(pattern);

}

void ReceiveHandler::setColor(const String& color)
{

      if(color == "green" || color == "Green" || color == "Grn" || color == "grn" || color == "GRN"){
        Serial.print("Set Color: "); Serial.println(color);
        this->ledsManager->setColor(CRGB::DarkGreen);
      }

      else if(color == "red" || color == "Red"  || color == "rot" || color == "Rot" || color == "ROT"){
        Serial.print("Set Color: "); Serial.println(color);
        this->ledsManager->setColor(CRGB::DarkRed);
      }

      else if(color == "blue" || color == "Blue"  || color == "Blau" || color == "blau" || color == "BLAU"){
        Serial.print("Set Color: "); Serial.println(color);
        this->ledsManager->setColor(CRGB::DeepSkyBlue);
      }

      else if(color == "yellow" || color == "Yellow"  || color == "Gelb" || color == "gelb" || color == "GELB"){
        Serial.print("Set Color: "); Serial.println(color);
        this->ledsManager->setColor(CRGB::Gold);
      }

      else if(color == "purple" || color == "Purple"  || color == "Lila" || color == "lila" || color == "LILA"){
        Serial.print("Set Color: "); Serial.println(color);
        this->ledsManager->setColor(CRGB::MediumPurple);
      }

      else if(color == "cyan" || color == "Cyan" || color == "CYAN" ){
        Serial.print("Set Color: "); Serial.println(color);
        this->ledsManager->setColor(CRGB::Cyan);
      }

      else if(color == "orange" || color == "Orange" || color == "ORANGE"){
        Serial.print("Set Color: "); Serial.println(color);
        this->ledsManager->setColor(CRGB::DarkOrange);
      }

      else if(color == "pink" || color == "Pink" || color == "PINK" ){
        Serial.print("Set Color: "); Serial.println(color);
        this->ledsManager->setColor(CRGB::DeepPink);
      }

       else if(color == "white" || color == "White" || color == "wei"  || color == "Wei" || color == "WEI"){
        Serial.print("Set Pattern: "); Serial.println(color);
        this->ledsManager->setColor(CRGB::NavajoWhite);
      }


}

void ReceiveHandler::setPattern(const String& pattern)
{
      if(pattern == "fade" || pattern == "Fade" ){
        Serial.print("Set Pattern: "); Serial.println(pattern);
        this->ledsManager->setPattern(FADE);
      }

      else if(pattern == "flash" || pattern == "Flash" ){
        Serial.print("Set Pattern: "); Serial.println(pattern);
        this->ledsManager->setPattern(FLASH);
      }

      else if(pattern == "move" || pattern == "Move" ){
        Serial.print("Set Pattern: "); Serial.println(pattern);
        this->ledsManager->setPattern(JUGGLE);
      }

      else if(pattern == "neuland" || pattern == "Neuland" ){
        Serial.print("Set Pattern: "); Serial.println(pattern);
        this->ledsManager->setPattern(RAINBOW);
      }

      else if(pattern == "solid" || pattern == "Solid" ){
        Serial.print("Set Pattern: "); Serial.println(pattern);
        this->ledsManager->setPattern(SOLID);
      }
}



