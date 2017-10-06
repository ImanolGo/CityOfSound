# City Of Sound

This sketch shows how to remote control wearable LEDs using text messages.

I've used the Adafruit Feather FONA. It has  with GSM capabilities and is able to receive and decode SMS.

## Installation

- We use 12mm Diffused Thin Digital RGB LED Pixels (Strand of 25) - WS2801.
- The pixels are connected by a 4-conductor cable. +5V (Red), Ground (Blue), Data (Yellow) and Clock (Green). 
- At 5 volts, they draw a maximum of 60 milliamps per pixel: 20 mA each for red, green and blue..
- 5V/2A supply should be able to drive 2 or more strands

// LED strand GND/0V/-      <----> Adafruit Feather GND 
// LED strand 5V/+          <---->  Adafruit Feather USB output  
// LED strand MOSI/Data in  <----> Adafruit Feather SPI MOSI (GPIO #13)
// LED strand SCLK/Clock    <----> Adafruit Feather SPI SCK  (GPIO #14) 

The firmware of the Adafruit Feather 32u4 FONA (Figure 2), was developed using the Arduino platform, the Adafruit_FONA library and the FastLED library.

## Usage

1. Turn on the device. The dress will blink white once.
2. Plug the power supply to the power socket or USB power bank.
3. Send an SMS with “#color” to the number: +4915735983480
4. Enjoy the new light pattern.

## Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :D

## History

TODO: Write history

## Credits

TODO: Write credits

## License

 This code is under A Creative Commons Attribution/Share-Alike License http://creativecommons.org/licenses/by-sa/4.0/ (2016) by Imanol Gomez
