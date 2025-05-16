#include <Arduino.h>
#include "FS.h"
#include "SPIFFS.h"
#include "FastLED.h"
#include <SimpleSerialShell.h>

#define BUTTON_PIN 0
#define RGB_DATA_PIN  14

#define NUM_LEDS 64


CRGB leds[NUM_LEDS];

/*
Shell Commands

test (Test command prompt)
dir  (List files)
run (Interacive basic shell)
run file
list file
load file  (Text after this will be safed to file system unitl a "break" character is received. Existing file will be overwritten)
delete file 
*/


/*
Commands
*/
int test(int argc, char **argv) 
{
  Serial.println("Test function called");
  return 0;
};

/*
LED Stuff
*/

void blink_leds(CRGB col)
{
    leds[0] = CRGB::Black;
    //Set LED's
    FastLED.show();
    delay(300);
    leds[0] = col;
    //Set LED's
    FastLED.show();
    delay(500);
    leds[0] = CRGB::Black;
    //Set LED's
    FastLED.show();
    delay(300);      
}

void color_leds(CRGB col)
{
    for (int ii=0;ii<NUM_LEDS;ii++)
    {
        leds[ii] = col;    
    }
}

/*
Main Code
*/


void setup() 
{
    // put your setup code here, to run once:

    pinMode(LED_BUILTIN, OUTPUT);
    blink_leds(CRGB::Red);
    FastLED.addLeds<WS2812B, RGB_DATA_PIN, GRB>(leds, NUM_LEDS);

    Serial.begin(115200);
    while (!Serial) 
    {
        // wait for serial port to connect. Needed for native USB port only
        // AND you want to block until there's a connection
        // otherwise the shell can quietly drop output.
    }

    //example
    shell.attach(Serial);
    shell.addCommand(F("test"), test);

    SPIFFS.begin(); // Initialize SPIFFS

    Serial.println(F("Ready."));
    blink_leds(CRGB::Green);
}

void loop() 
{
  shell.executeIfInput();
}

