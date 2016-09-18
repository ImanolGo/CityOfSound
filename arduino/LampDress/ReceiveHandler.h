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
  String thirdString = message.substring(secondHashTagIndex); // To the end of the string
  
}

void ReceiveHandler::setCommands(const String& color, const String& pattern)
{
      this->setColor(color);
      this->setPattern(pattern);

}

void ReceiveHandler::setColor(const String& color)
{

      if(color == "green" || color == "Green" ){
        this->ledsManager->setColor(CRGB::DarkGreen);
      }

      else if(color == "red" || color == "Red" ){
        this->ledsManager->setColor(CRGB::DarkRed);
      }

      else if(color == "blue" || color == "Blue" ){
        this->ledsManager->setColor(CRGB::DodgerBlue);
      }

      else if(color == "blue" || color == "Blue" ){
        this->ledsManager->setColor(CRGB::DodgerBlue);
      }

      else if(color == "yellow" || color == "Yellow" ){
        this->ledsManager->setColor(CRGB::Gold);
      }

      else if(color == "purple" || color == "Purple" ){
        this->ledsManager->setColor(CRGB::MediumPurple);
      }

      else if(color == "cyan" || color == "Cyan" ){
        this->ledsManager->setColor(CRGB::Cyan);
      }

      else if(color == "orange" || color == "Orange" ){
        this->ledsManager->setColor(CRGB::DarkOrange);
      }

      else if(color == "pink" || color == "Pink" ){
        this->ledsManager->setColor(CRGB::DeepPink);
      }

}

void ReceiveHandler::setPattern(const String& pattern)
{
      if(pattern == "fade" || pattern == "Fade" ){
        this->ledsManager->setPattern(SINELON);
      }

      else if(pattern == "flash" || pattern == "Flash" ){
        this->ledsManager->setPattern(BPM);
      }

      else if(pattern == "move" || pattern == "Move" ){
        this->ledsManager->setPattern(JUGGLE);
      }

      else if(pattern == "neuland" || pattern == "Neuland" ){
        this->ledsManager->setPattern(RAINBOW);
      }
}



