///////////////////////////////////////////////////////////////////
// Class controlling the leds and its effects
//
// Most of th eanimations were based on Mark Kriegsman's demo reel, December 2014
//
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2016, Imanol Gomez
///////////////////////////////////////////////////////////////////

#pragma once
#include "Arduino.h"
#include "FastLED.h"


FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif


#define NUM_LEDS    50
#define BRIGHTNESS  84
#define LED_TYPE    WS2801
#define COLOR_ORDER RGB
#define DATA_PIN 11
#define CLOCK_PIN 10
#define FRAMES_PER_SECOND  120
#define NUM_PATTERNS  4

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


enum LED_PATTERNS {
  SOLID_GLITTER,
  JUGGLE,
  FADE,
  FLASH,
  RAINBOW,
  RAINBOW_GLITTER,
  CONFETTI,
  SINELON,
  BPM,
  SOLID,
  SLOW_JUGGLE
};


class LedsManager{

  public:
    
    LedsManager() {}
    
    void setup();
    void update();

    void setColor(CRGB  color) {gColor = color;}
    void setPattern(uint8_t patternNumber );
    void nextPattern();
    void setDefaultState();

  private:

    // List of patterns to cycle through.  Each is defined as a separate function below.
    //typedef void (*SimplePatternList[])();
    //SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

    void rainbow();
    void rainbowWithGlitter();
    void addGlitter( fract8 chanceOfGlitter);
    void confetti();
    void sinelon();
    void bpm();
    void juggle();
    void slow_juggle();
    void runPattern();
    void solid();
    void solidWithGlitter();
    void fade();
    void flash();
    void rainbowCycle() ;
    byte * Wheel(byte WheelPos);
    
    CRGB leds[NUM_LEDS]; // Define the array of leds
    uint8_t gCurrentPatternNumber; // Index number of which pattern is current
    uint8_t gHue; // rotating "base color" used by many of the patterns
    CRGB    gColor;

    uint8_t fadeAmount;  // Set the amount to fade I usually do 5, 10, 15, 20, 25 etc even up to 255.
    uint8_t flashAmount;  // Set the amount to fade I usually do 5, 10, 15, 20, 25 etc even up to 255.
    uint8_t fadeBrightness;
    float cycleIndex;

};

void LedsManager::setup()
{
    delay(1000); // 3 second delay for recovery
  
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setMaxPowerInVoltsAndMilliamps(5,2100); 
    //FastLED.setBrightness(  BRIGHTNESS );
  

    gCurrentPatternNumber = SOLID; 
    gColor = CRGB::Black;
    gHue = 0; 

    fadeAmount = 1;  // Set the amount to fade I usually do 5, 10, 15, 20, 25 etc even up to 255.
    flashAmount = 16; 
    fadeBrightness =0;
    cycleIndex = 0;

    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::White;
     }
    FastLED.show();

    delay(1500); // 3 second delay for recovery
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
     }
    FastLED.show();
    
    this->setDefaultState();
}

void LedsManager::update()
{

    // Call the current pattern function once, updating the 'leds' array
    //gPatterns[gCurrentPatternNumber]();

    runPattern();

    // send the 'leds' array out to the actual LED strip
    FastLED.show();  
    // insert a delay to keep the framerate modest
    FastLED.delay(1000/FRAMES_PER_SECOND); 
  
    // do some periodic updates
   // EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
    //EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}


void LedsManager::setDefaultState()
{
    //setColor(CRGB::SandyBrown);
    setPattern(RAINBOW);
}
void LedsManager::runPattern()
{

    switch (gCurrentPatternNumber) 
    {
        case RAINBOW:
          rainbow();
          break;
        case RAINBOW_GLITTER:
          rainbowWithGlitter();
          break;
        case CONFETTI:
          confetti();
          break;
        case SINELON:
          sinelon();
          break;
        case JUGGLE:
          juggle();
          break;
        case BPM:
          bpm();
          break;
        case SOLID:
          solid();
          break;
        case SOLID_GLITTER:
          solidWithGlitter();
          break;
        case FADE:
          fade();
          break;
        case FLASH:
          flash();
          break;
        case SLOW_JUGGLE:
          slow_juggle();
          break;
          
        default: 
          // if nothing else matches, do the default
          // default is optional
        break;
    }
}

void LedsManager::setPattern(uint8_t patternNumber )
{
    gCurrentPatternNumber = patternNumber;
}

void LedsManager::nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % NUM_PATTERNS;
}

void LedsManager::solid()
{
   fill_solid(leds, NUM_LEDS, gColor);  
}


void LedsManager::solidWithGlitter()
{
   solid();
   addGlitter(80);
}


 
void LedsManager::rainbow() 
{
  // FastLED's built-in rainbow generator
  //fill_rainbow( leds, NUM_LEDS, gHue, 7);

  rainbowCycle();
}

void LedsManager::rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void LedsManager::addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void LedsManager::confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void LedsManager::sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += gColor;
}

void LedsManager::fade()
{
   uint16_t bpm = 10; 
   for(int i = 0; i < NUM_LEDS; i++ )
   {
     leds[i] = gColor;
     leds[i].fadeLightBy(beatsin16(bpm,0,255));
   }
    
}

void LedsManager::flash()
{
   uint16_t bpm = 240;
   
   for(int i = 0; i < NUM_LEDS; i++ )
   {
     leds[i] = gColor;
    // CRGB color = CHSV( colorHSV.hue, colorHSV.saturation, beat16(bpm));
     leds[i].fadeLightBy(beat16(bpm));
   }
}

void LedsManager::bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void LedsManager::juggle() 
{
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= gColor;
  }
}

void LedsManager::slow_juggle() 
{
   for(int i = 0; i < NUM_LEDS; i++ )
   {
     leds[i] = gColor;
     leds[i].fadeLightBy(beatsin16(10,0,255));
   }
   
}



 
void LedsManager::rainbowCycle() 
{
  byte *c;
  uint16_t factor = (uint16_t) floor(cycleIndex);
  factor = factor % (256*5);
  
  for(uint16_t i=0; i< NUM_LEDS; i++) {
      c=Wheel(((i * 256 / NUM_LEDS) + factor) & 255);
      leds[i].r = *c;
      leds[i].g =  *(c+1);
      leds[i].b =  *(c+2);
    }

  cycleIndex +=0.5;
 
  
}

byte * LedsManager::Wheel(byte WheelPos) {
  static byte c[3];
  
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}

