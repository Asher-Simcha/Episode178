// We're including two Libraries, "Ultrasonic.h" and "FastLED.h"
// FastLED.h gives us the ability to addres the WS2812 LEDs
// "Ultrasonic.h" allows us to trigger and read the ultrasonic sensor
#include "Ultrasonic.h"
#include "FastLED.h"

// How many LEDs will you have in your array?
#define NUM_LEDS 8

// Which Digital pin will be used to control the WS2812 lights?
// We'll be using Digital Data Pin #7
#define DATA_PIN 3

// Creates an array with the length set by NUM_LEDS above
// This is the array the library will read to determine how each LED in the strand should be set
CRGB leds[NUM_LEDS];

// Now we're going to initialize the Ultrasonic Sensor
// The sensor itself needs four leads: ground, vcc, trigger, and echo
// This tells the ultrasonic library that the trigger pin is on digital 11
// This also tells the ultrasonic library that the echo pin is on digital 10
Ultrasonic ultrasonic(13,12);

// This tells the library that there is a strand of NEOPIXELS attached to the Arduino
// that they are connected to the DATA_PIN specified above
// that the library should look at the array called "leds"
// and that there are the number of LEDs in the strand as defined above in "NUM_LEDS" 
// It also opens the serial port so we can see the range values 
void setup()
{
  FastLED.addLeds<NEOPIXEL,DATA_PIN>(leds, NUM_LEDS);
  Serial.begin(9600);
}

// In our last code example, we used the serial console to report the position of our analog potentiometer.
// This time, we're using it to report the Only used for debugging. This will report the distance of the ultrasonic sensor to an object hard enough to create an echo
void Range_Debug(int x)
{
  Serial.println(x);
  delay(200);
}

void Range_LED(int RangeValue)
{
    // This creates an integer named "LightMap" that remaps a given range of distances, in CM, to the range of leds
    // the map function uses the following values (the number to map, the lower boundary of the current value, the upper boundary of the current value, the lower boundary of the target, the upper boundary of the target)
    // Even though our US sensor was detecting out to 170CM, I'm compressing it so that it will fit better into the LEDs
    // I've made NUM_LEDS the lower boundary of the target, because I want ALL leds to be lit when the range value is lowest.
    int LightMap = map(RangeValue, 0, 100, NUM_LEDS, 0);
 
    // Resets the LED values
    FastLED.clear();
 
    // Change led colors based on the value of "RangeValue"
    // Starts with blue, then green, then orange, then red. Two LEDs for each color
    for(int led = 0; led < LightMap; led++) 
    { 
      if(led < (NUM_LEDS * .25))                             leds[led] = CRGB::Blue;
      if(led >= (NUM_LEDS * .25) & led < (NUM_LEDS * .5))    leds[led] = CRGB::Green;
      if(led >= (NUM_LEDS * .5) & led < (NUM_LEDS * .75))    leds[led] = CRGB::Yellow;
      if(led >= (NUM_LEDS * .75) & led < (NUM_LEDS))         leds[led] = CRGB::Red;
     }
 
     FastLED.setBrightness(60);
     FastLED.show();
}

void loop() {
  // This creates an integer variable called "range", then fills it with the range from the Ultrasonic sensor in centimeters
  int range=(ultrasonic.Ranging(CM));

  // This calls the Range_Debug function and passes it the value of the "range" variable
  // This allows us to see the range values on the serial port monitor
  Range_Debug(range);

  // This calls our LED function to represent the range from target in LED lights
  Range_LED(range);
  delay(100);
}
