//
//  FastLED.hpp
//  FastLED
//
//  Created by Andreas Pleschutznig on 4/14/16.
//  Copyright © 2016 Andreas Pleschutznig. All rights reserved.
//

#ifndef FastLED_hpp
#define FastLED_hpp

#include <stdio.h>
#include <stdint.h>
#include <string.h>         //strlen
#include <sys/socket.h>     //socket
#include <arpa/inet.h>      //inet_addr

#include "lib8tion.h"
#include "pixeltypes.h"
#include "FastledDefinitions.h"
#include "colorutils.h"

extern int connect8266(char *, uint16_t);
extern int RleEncodePass2(unsigned char *, unsigned int, unsigned char *, unsigned int *);
extern int delay(uint16_t);

class NetworkLed {
private:
    unsigned char Header[3];
    unsigned char StartOfText[2];
    unsigned char EndOfText[2];
public:
    int sock;
    uint16_t NumLeds;
    CRGB *leds;
    
public:
    NetworkLed(void) {
    }
    
    int Connect(char *ip) {
        sock = connect8266((char *)ip, (uint16_t) 0xfa57);
        return(sock);
    }
    
    void setStore(struct CRGB * l) {
        this->leds = l;
    }
    
    void clear() {
        for (int i = 0; i < NumLeds; i++)
            leds[i] = 0;
    }
    
    void fill_solid( struct CRGB * leds, int numToFill,
                    const struct CRGB& color)
    {
        for( int i = 0; i < numToFill; i++) {
            leds[i] = color;
        }
    }
    
    void fill_solid( struct CHSV * targetArray, int numToFill,
                    const struct CHSV& hsvColor)
    {
        for( int i = 0; i < numToFill; i++) {
            targetArray[i] = hsvColor;
        }
    }
    
    // void fill_solid( struct CRGB* targetArray, int numToFill,
    // 				 const struct CHSV& hsvColor)
    // {
    // 	fill_solid<CRGB>( targetArray, numToFill, (CRGB) hsvColor);
    // }
    
    void fill_rainbow( struct CRGB * pFirstLED, int numToFill,
                      uint8_t initialhue,
                      uint8_t deltahue )
    {
        CHSV hsv;
        hsv.hue = initialhue;
        hsv.val = 255;
        hsv.sat = 240;
        for( int i = 0; i < numToFill; i++) {
            pFirstLED[i] = hsv;
            hsv.hue += deltahue;
        }
    }
    
    void fill_rainbow( struct CHSV * targetArray, int numToFill,
                      uint8_t initialhue,
                      uint8_t deltahue )
    {
        CHSV hsv;
        hsv.hue = initialhue;
        hsv.val = 255;
        hsv.sat = 240;
        for( int i = 0; i < numToFill; i++) {
            targetArray[i] = hsv;
            hsv.hue += deltahue;
        }
    }
    
    
    void fill_gradient_RGB( CRGB* leds, uint16_t numLeds, const CRGB& c1, const CRGB& c2)
    {
        uint16_t last = numLeds - 1;
        fill_gradient_RGB( leds, 0, c1, last, c2);
    }
    
    
    void fill_gradient_RGB( CRGB* leds, uint16_t numLeds, const CRGB& c1, const CRGB& c2, const CRGB& c3)
    {
        uint16_t half = (numLeds / 2);
        uint16_t last = numLeds - 1;
        fill_gradient_RGB( leds,    0, c1, half, c2);
        fill_gradient_RGB( leds, half, c2, last, c3);
    }
    
    void fill_gradient_RGB( CRGB* leds, uint16_t numLeds, const CRGB& c1, const CRGB& c2, const CRGB& c3, const CRGB& c4)
    {
        uint16_t onethird = (numLeds / 3);
        uint16_t twothirds = ((numLeds * 2) / 3);
        uint16_t last = numLeds - 1;
        fill_gradient_RGB( leds,         0, c1,  onethird, c2);
        fill_gradient_RGB( leds,  onethird, c2, twothirds, c3);
        fill_gradient_RGB( leds, twothirds, c3,      last, c4);
    }
    
    
    
    void nscale8_video( CRGB* leds, uint16_t num_leds, uint8_t scale)
    {
        for( uint16_t i = 0; i < num_leds; i++) {
            leds[i].nscale8_video( scale);
        }
    }
    
    void fade_video(CRGB* leds, uint16_t num_leds, uint8_t fadeBy)
    {
        nscale8_video( leds, num_leds, 255 - fadeBy);
    }
    
    void fadeLightBy(CRGB* leds, uint16_t num_leds, uint8_t fadeBy)
    {
        nscale8_video( leds, num_leds, 255 - fadeBy);
    }
    
    
    void fadeToBlackBy( CRGB* leds, uint16_t num_leds, uint8_t fadeBy)
    {
        nscale8( leds, num_leds, 255 - fadeBy);
    }
    
    void fade_raw( CRGB* leds, uint16_t num_leds, uint8_t fadeBy)
    {
        nscale8( leds, num_leds, 255 - fadeBy);
    }
    
    void nscale8_raw( CRGB* leds, uint16_t num_leds, uint8_t scale)
    {
        nscale8( leds, num_leds, scale);
    }
    
    void nscale8( CRGB* leds, uint16_t num_leds, uint8_t scale)
    {
        for( uint16_t i = 0; i < num_leds; i++) {
            leds[i].nscale8( scale);
        }
    }
    
    void fadeUsingColor( CRGB* leds, uint16_t numLeds, const CRGB& colormask)
    {
        uint8_t fr, fg, fb;
        fr = colormask.r;
        fg = colormask.g;
        fb = colormask.b;
        
        for( uint16_t i = 0; i < numLeds; i++) {
            leds[i].r = scale8_LEAVING_R1_DIRTY( leds[i].r, fr);
            leds[i].g = scale8_LEAVING_R1_DIRTY( leds[i].g, fg);
            leds[i].b = scale8                 ( leds[i].b, fb);
        }
    }

    
    CRGB& nblend( CRGB& existing, const CRGB& overlay, fract8 amountOfOverlay )
    {
        if( amountOfOverlay == 0) {
            return existing;
        }
        
        if( amountOfOverlay == 255) {
            existing = overlay;
            return existing;
        }
        
        fract8 amountOfKeep = 255 - amountOfOverlay;
        
        existing.red   = scale8_LEAVING_R1_DIRTY( existing.red,   amountOfKeep)
        + scale8_LEAVING_R1_DIRTY( overlay.red,    amountOfOverlay);
        existing.green = scale8_LEAVING_R1_DIRTY( existing.green, amountOfKeep)
        + scale8_LEAVING_R1_DIRTY( overlay.green,  amountOfOverlay);
        existing.blue  = scale8_LEAVING_R1_DIRTY( existing.blue,  amountOfKeep)
        + scale8_LEAVING_R1_DIRTY( overlay.blue,   amountOfOverlay);
        
        cleanup_R1();
        
        return existing;
    }
    
    
    
    void nblend( CRGB* existing, CRGB* overlay, uint16_t count, fract8 amountOfOverlay)
    {
        for( uint16_t i = count; i; i--) {
            nblend( *existing, *overlay, amountOfOverlay);
            existing++;
            overlay++;
        }
    }
    
    CRGB blend( const CRGB& p1, const CRGB& p2, fract8 amountOfP2 )
    {
        CRGB nu(p1);
        nblend( nu, p2, amountOfP2);
        return nu;
    }
    
    CRGB* blend( const CRGB* src1, const CRGB* src2, CRGB* dest, uint16_t count, fract8 amountOfsrc2 )
    {
        for( uint16_t i = 0; i < count; i++) {
            dest[i] = blend(src1[i], src2[i], amountOfsrc2);
        }
        return dest;
    }
    
    
    
    CHSV& nblend( CHSV& existing, const CHSV& overlay, fract8 amountOfOverlay, TGradientDirectionCode directionCode)
    {
        if( amountOfOverlay == 0) {
            return existing;
        }
        
        if( amountOfOverlay == 255) {
            existing = overlay;
            return existing;
        }
        
        fract8 amountOfKeep = 255 - amountOfOverlay;
        
        uint8_t huedelta8 = overlay.hue - existing.hue;
        
        if( directionCode == SHORTEST_HUES ) {
            directionCode = FORWARD_HUES;
            if( huedelta8 > 127) {
                directionCode = BACKWARD_HUES;
            }
        }
        
        if( directionCode == LONGEST_HUES ) {
            directionCode = FORWARD_HUES;
            if( huedelta8 < 128) {
                directionCode = BACKWARD_HUES;
            }
        }
        
        if( directionCode == FORWARD_HUES) {
            existing.hue = existing.hue + scale8( huedelta8, amountOfOverlay);
        }
        else /* directionCode == BACKWARD_HUES */
        {
            huedelta8 = -huedelta8;
            existing.hue = existing.hue - scale8( huedelta8, amountOfOverlay);
        }
        
        existing.sat   = scale8_LEAVING_R1_DIRTY( existing.sat,   amountOfKeep)
        + scale8_LEAVING_R1_DIRTY( overlay.sat,    amountOfOverlay);
        existing.val = scale8_LEAVING_R1_DIRTY( existing.val, amountOfKeep)
        + scale8_LEAVING_R1_DIRTY( overlay.val,  amountOfOverlay);
        
        cleanup_R1();
        
        return existing;
    }
    
    
    
    void nblend( CHSV* existing, CHSV* overlay, uint16_t count, fract8 amountOfOverlay, TGradientDirectionCode directionCode )
    {
        if(existing == overlay) return;
        for( uint16_t i = count; i; i--) {
            nblend( *existing, *overlay, amountOfOverlay, directionCode);
            existing++;
            overlay++;
        }
    }
    
    CHSV blend( const CHSV& p1, const CHSV& p2, fract8 amountOfP2, TGradientDirectionCode directionCode )
    {
        CHSV nu(p1);
        nblend( nu, p2, amountOfP2, directionCode);
        return nu;
    }
    
    CHSV* blend( const CHSV* src1, const CHSV* src2, CHSV* dest, uint16_t count, fract8 amountOfsrc2, TGradientDirectionCode directionCode )
    {
        for( uint16_t i = 0; i < count; i++) {
            dest[i] = blend(src1[i], src2[i], amountOfsrc2, directionCode);
        }
        return dest;
    }
    
    
    
    // Forward declaration of the function "XY" which must be provided by
    // the application for use in two-dimensional filter functions.
    uint16_t XY( uint8_t, uint8_t);// __attribute__ ((weak));
    
    
    // blur1d: one-dimensional blur filter. Spreads light to 2 line neighbors.
    // blur2d: two-dimensional blur filter. Spreads light to 8 XY neighbors.
    //
    //           0 = no spread at all
    //          64 = moderate spreading
    //         172 = maximum smooth, even spreading
    //
    //         173..255 = wider spreading, but increasing flicker
    //
    //         Total light is NOT entirely conserved, so many repeated
    //         calls to 'blur' will also result in the light fading,
    //         eventually all the way to black; this is by design so that
    //         it can be used to (slowly) clear the LEDs to black.
    void blur1d( CRGB* leds, uint16_t numLeds, fract8 blur_amount)
    {
        uint8_t keep = 255 - blur_amount;
        uint8_t seep = blur_amount >> 1;
        CRGB carryover = CRGB::Black;
        for( uint16_t i = 0; i < numLeds; i++) {
            CRGB cur = leds[i];
            CRGB part = cur;
            part.nscale8( seep);
            cur.nscale8( keep);
            cur += carryover;
            if( i) leds[i-1] += part;
            leds[i] = cur;
            carryover = part;
        }
    }
    
    void blur2d( CRGB* leds, uint8_t width, uint8_t height, fract8 blur_amount)
    {
        blurRows(leds, width, height, blur_amount);
        blurColumns(leds, width, height, blur_amount);
    }
    
    
    // blurRows: perform a blur1d on every row of a rectangular matrix
    void blurRows(CRGB* leds, uint8_t width, uint8_t height, fract8 blur_amount)
    {
        // blur rows
        uint8_t keep = 255 - blur_amount;
        uint8_t seep = blur_amount >> 1;
        for( uint8_t rows = 0; rows < height; rows++) {
            CRGB carryover = CRGB::Black;
            for( uint8_t i = 0; i < width; i++) {
                CRGB cur = leds[XY(i, rows)];
                CRGB part = cur;
                part.nscale8( seep);
                cur.nscale8( keep);
                cur += carryover;
                if( i) leds[XY(i-1,rows)] += part;
                leds[XY(i,rows)] = cur;
                carryover = part;
            }
        }
    }
    
    // blurColumns: perform a blur1d on each column of a rectangular matrix
    void blurColumns(CRGB* leds, uint8_t width, uint8_t height, fract8 blur_amount)
    {
        // blur columns
        uint8_t keep = 255 - blur_amount;
        uint8_t seep = blur_amount >> 1;
        for( uint8_t col = 0; col < width; col++) {
            CRGB carryover = CRGB::Black;
            for( uint8_t i = 0; i < height; i++) {
                CRGB cur = leds[XY(col,i)];
                CRGB part = cur;
                part.nscale8( seep);
                cur.nscale8( keep);
                cur += carryover;
                if( i) leds[XY(col,i-1)] += part;
                leds[XY(col,i)] = cur;
                carryover = part;
            }
        }
    }
    
    
    // CRGB HeatColor( uint8_t temperature)
    //
    // Approximates a 'black body radiation' spectrum for
    // a given 'heat' level.  This is useful for animations of 'fire'.
    // Heat is specified as an arbitrary scale from 0 (cool) to 255 (hot).
    // This is NOT a chromatically correct 'black body radiation'
    // spectrum, but it's surprisingly close, and it's fast and small.
    //
    // On AVR/Arduino, this typically takes around 70 bytes of program memory,
    // versus 768 bytes for a full 256-entry RGB lookup table.
    
    CRGB HeatColor( uint8_t temperature)
    {
        CRGB heatcolor;
        
        // Scale 'heat' down from 0-255 to 0-191,
        // which can then be easily divided into three
        // equal 'thirds' of 64 units each.
        uint8_t t192 = scale8_video( temperature, 192);
        
        // calculate a value that ramps up from
        // zero to 255 in each 'third' of the scale.
        uint8_t heatramp = t192 & 0x3F; // 0..63
        heatramp <<= 2; // scale up to 0..252
        
        // now figure out which third of the spectrum we're in:
        if( t192 & 0x80) {
            // we're in the hottest third
            heatcolor.r = 255; // full red
            heatcolor.g = 255; // full green
            heatcolor.b = heatramp; // ramp up blue
            
        } else if( t192 & 0x40 ) {
            // we're in the middle third
            heatcolor.r = 255; // full red
            heatcolor.g = heatramp; // ramp up green
            heatcolor.b = 0; // no blue
            
        } else {
            // we're in the coolest third
            heatcolor.r = heatramp; // ramp up red
            heatcolor.g = 0; // no green
            heatcolor.b = 0; // no blue
        }
        
        return heatcolor;
    }


    
    void SetNumLeds(uint16_t num) {
        unsigned char outMessage[20];
        
        NumLeds = num;
        
        uint16_t xfer;
        
        outMessage[0] = (unsigned char) SYN;
        outMessage[1] = (unsigned char) SOH;
        outMessage[2] = (unsigned char) STX;
        outMessage[3] = (unsigned char) FastledSetNumLeds;
        
        // might have to change byte order here
        
        xfer = htons(num);
        memcpy(&outMessage[4], &xfer, 2);
                
        if( send(sock , outMessage , 6 , 0) < 0)
        {
            puts("Send failed");
            //            return 1;
        }

    }
    
    void setBrightness(uint8_t num) {
        unsigned char outMessage[20];
        
        outMessage[0] = (unsigned char) SYN;
        outMessage[1] = (unsigned char) SOH;
        outMessage[2] = (unsigned char) STX;
        outMessage[3] = (unsigned char) FastledSetBrightness;
        outMessage[4] = (unsigned char) num;
        
        if( send(sock , outMessage , 5 , 0) < 0)
        {
            puts("Send failed");
            //            return 1;
        }
        
    }

    
    void show() {
        unsigned char outMessage[20];
        
        outMessage[0] = (unsigned char) SYN;
        outMessage[1] = (unsigned char) SOH;
        outMessage[2] = (unsigned char) STX;
        outMessage[3] = (unsigned char) FastledShow;
        
        if( send(sock , outMessage , 4 , 0) < 0)
        {
            puts("Send failed");
            //            return 1;
        }

        
    }
    
    void transfer() {
        unsigned char header;
        unsigned int  outLength2;
        unsigned char rleMessage2[6000], outMessage[6100];
        unsigned char *rleMessage = { };
        unsigned int outLength;
        uint16_t numBytes;              // How long is the message?
        uint16_t xfer;
        
        // We will work under the assumption that since we always have to transfer the whole CRGB[]
        // the most expensive part of this transaction is the physical transfer vie the network
        // Further we assume that the host we are on has enough horsepower available to run both encoders
        // and after that we will check which transfer is cheaper and select this one, marking in the header
        // for the microcontroller, so it can be properly decoded.
        
//        RleEncodePass1((unsigned char *)leds, NumLeds*3, rleMessage1, &outLength1);
        RleEncodePass2((unsigned char *)leds, NumLeds*3, rleMessage2, &outLength2);
        //Send some data
        if ( (NumLeds*3) < outLength2){
            header = UNCOMPRESSED;
            rleMessage = (unsigned char *)leds;
            outLength = NumLeds*3;
        } else {
            header = PHASE2;
            rleMessage = rleMessage2;
            outLength = outLength2;
        }
        outMessage[0] = (unsigned char) SYN;
        outMessage[1] = (unsigned char) SOH;
        outMessage[2] = (unsigned char) STX;
        outMessage[3] = header;
        numBytes = outLength;
        xfer = htons(numBytes);
        memcpy(&outMessage[4], &xfer, 2);
        
        memcpy(&outMessage[6], rleMessage, outLength );                         // send packet identifier
        if( send(sock , outMessage , outLength + 6 , 0) < 0)
        {
            puts("Send failed");
            //            return 1;
        }
        
        
        
    }
    
};



#endif /* FastLED_hpp */
