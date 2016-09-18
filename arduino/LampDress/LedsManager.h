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


#define NUM_LEDS    25
#define BRIGHTNESS  84
#define LED_TYPE    WS2801
#define COLOR_ORDER RGB
#define DATA_PIN 10
#define CLOCK_PIN 11
#define FRAMES_PER_SECOND  120
#define NUM_PATTERNS  10

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


enum LED_PATTERNS {
  RAINBOW,
  RAINBOW_GLITTER,
  CONFETTI,
  SINELON,
  JUGGLE,
  BPM,
  SOLID,
  SOLID_GLITTER,
  FADE,
  FLASH
};


class LedsManager{

  public:
    
    LedsManager() {}
    
    void setup();
    void update();

    void setColor(CRGB  color) {gColor = color;}
    void setPattern(uint8_t patternNumber );
    void nextPattern();

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
    void runPattern();
    void solid();
    void solidWithGlitter();
    void fade();
    void flash();
    
    CRGB leds[NUM_LEDS]; // Define the array of leds
    uint8_t gCurrentPatternNumber; // Index number of which pattern is current
    uint8_t gHue; // rotating "base color" used by many of the patterns
    CRGB    gColor;

    int fadeAmount;  // Set the amount to fade I usually do 5, 10, 15, 20, 25 etc even up to 255.
    int fadeBrightness;

};

void LedsManager::setup()
{
    delay(3000); // 3 second delay for recovery
  
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setMaxPowerInVoltsAndMilliamps(5,2100); 
    FastLED.setBrightness(  BRIGHTNESS );
    FastLED.show();

    gCurrentPatternNumber = 0; 
    gHue = 0; 

    fadeAmount = 5;  // Set the amount to fade I usually do 5, 10, 15, 20, 25 etc even up to 255.
    fadeBrightness = 0;
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
          fade();
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
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
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
   fadeAmount = 5;
   for(int i = 0; i < NUM_LEDS; i++ )
   {
     leds[i].fadeLightBy(fadeBrightness);
   }

   fadeBrightness = fadeBrightness + fadeAmount;
  // reverse the direction of the fading at the ends of the fade:
  if(fadeBrightness <= 0 || fadeBrightness >= 255)
  {
    fadeAmount = -fadeAmount ;
  }   
}

void LedsManager::flash()
{
   fadeAmount = 50;
   for(int i = 0; i < NUM_LEDS; i++ )
   {
     leds[i].fadeLightBy(fadeBrightness);
   }

   fadeBrightness = fadeBrightness + fadeAmount;
  // reverse the direction of the fading at the ends of the fade:
  if(fadeBrightness <= 0 || fadeBrightness >= 255)
  {
    fadeAmount = -fadeAmount ;
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

void LedsManager::juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= gColor;
  }
}
