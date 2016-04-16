//
//  main.cpp
//  FastLED
//
//  Created by Andreas Pleschutznig on 4/14/16.
//  Copyright © 2016 Andreas Pleschutznig. All rights reserved.
//

#include <stdio.h>          //printf


#include "FastLED.hpp"

CRGB leds[100];

NetworkLed strip1;


void setup() {
    strip1.NumLeds = 10;
    strip1.setStore(leds);
    
    strip1.Connect((char *)"10.0.1.11");
    strip1.SetNumLeds(100);
    strip1.setBrightness(40);
    strip1.clear();
    
    for (int i = 0; i < 40; i++)
        leds[i] = CRGB(0xff,0,0);
}

void loop() {
    static uint8_t gHue = 0;
    for ( int i = 0; i < 100; i++) {
        strip1.fadeToBlackBy(leds, 100, 50);
        leds[i] = CHSV(gHue++, 240,240);
        strip1.transfer();
        strip1.show();
        delay(20);
    }
}

int main(int argc , char *argv[])
{
    setup();
    while (1) loop();
    return 0;
}
