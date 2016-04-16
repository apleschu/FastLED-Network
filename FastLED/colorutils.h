//
//  colorutils.h
//  FastLED
//
//  Created by Andreas Pleschutznig on 4/16/16.
//  Copyright © 2016 Andreas Pleschutznig. All rights reserved.
//

#ifndef colorutils_h
#define colorutils_h

///@file colorutils.h
/// functions for color fill, paletters, blending, and more

#include "FastLED.h"
#include "pixeltypes.h"

///@defgroup Colorutils Color utility functions
///A variety of functions for working with color, palletes, and leds
///@{

/// fill_solid -   fill a range of LEDs with a solid color
///                Example: fill_solid( leds, NUM_LEDS, CRGB(50,0,200));
void fill_solid( struct CRGB * leds, int numToFill,
                const struct CRGB& color);

/// fill_solid -   fill a range of LEDs with a solid color
///                Example: fill_solid( leds, NUM_LEDS, CRGB(50,0,200));
void fill_solid( struct CHSV* targetArray, int numToFill,
                const struct CHSV& hsvColor);


/// fill_rainbow - fill a range of LEDs with a rainbow of colors, at
///                full saturation and full value (brightness)
void fill_rainbow( struct CRGB * pFirstLED, int numToFill,
                  uint8_t initialhue,
                  uint8_t deltahue = 5);

/// fill_rainbow - fill a range of LEDs with a rainbow of colors, at
///                full saturation and full value (brightness)
void fill_rainbow( struct CHSV * targetArray, int numToFill,
                  uint8_t initialhue,
                  uint8_t deltahue = 5);


// fill_gradient - fill an array of colors with a smooth HSV gradient
//                 between two specified HSV colors.
//                 Since 'hue' is a value around a color wheel,
//                 there are always two ways to sweep from one hue
//                 to another.
//                 This function lets you specify which way you want
//                 the hue gradient to sweep around the color wheel:
//                   FORWARD_HUES: hue always goes clockwise
//                   BACKWARD_HUES: hue always goes counter-clockwise
//                   SHORTEST_HUES: hue goes whichever way is shortest
//                   LONGEST_HUES: hue goes whichever way is longest
//                 The default is SHORTEST_HUES, as this is nearly
//                 always what is wanted.
//
// fill_gradient can write the gradient colors EITHER
//     (1) into an array of CRGBs (e.g., into leds[] array, or an RGB Palette)
//   OR
//     (2) into an array of CHSVs (e.g. an HSV Palette).
//
//   In the case of writing into a CRGB array, the gradient is
//   computed in HSV space, and then HSV values are converted to RGB
//   as they're written into the RGB array.

typedef enum { FORWARD_HUES, BACKWARD_HUES, SHORTEST_HUES, LONGEST_HUES } TGradientDirectionCode;


// fadeLightBy and fade_video - reduce the brightness of an array
//                              of pixels all at once.  Guaranteed
//                              to never fade all the way to black.
//                              (The two names are synonyms.)
void fadeLightBy(   CRGB* leds, uint16_t num_leds, uint8_t fadeBy);
void fade_video(    CRGB* leds, uint16_t num_leds, uint8_t fadeBy);

// nscale8_video - scale down the brightness of an array of pixels
//                 all at once.  Guaranteed to never scale a pixel
//                 all the way down to black, unless 'scale' is zero.
void nscale8_video( CRGB* leds, uint16_t num_leds, uint8_t scale);

// fadeToBlackBy and fade_raw - reduce the brightness of an array
//                              of pixels all at once.  These
//                              functions will eventually fade all
//                              the way to black.
//                              (The two names are synonyms.)
void fadeToBlackBy( CRGB* leds, uint16_t num_leds, uint8_t fadeBy);
void fade_raw(      CRGB* leds, uint16_t num_leds, uint8_t fadeBy);

// nscale8 - scale down the brightness of an array of pixels
//           all at once.  This function can scale pixels all the
//           way down to black even if 'scale' is not zero.
void nscale8(       CRGB* leds, uint16_t num_leds, uint8_t scale);

// fadeUsingColor - scale down the brightness of an array of pixels,
//                  as though it were seen through a transparent
//                  filter with the specified color.
//                  For example, if the colormask is
//                    CRGB( 200, 100, 50)
//                  then the pixels' red will be faded to 200/256ths,
//                  their green to 100/256ths, and their blue to 50/256ths.
//                  This particular example give a 'hot fade' look,
//                  with white fading to yellow, then red, then black.
//                  You can also use colormasks like CRGB::Blue to
//                  zero out the red and green elements, leaving blue
//                  (largely) the same.
void fadeUsingColor( CRGB* leds, uint16_t numLeds, const CRGB& colormask);


// Pixel blending
//
// blend - computes a new color blended some fraction of the way
//         between two other colors.
CRGB  blend( const CRGB& p1, const CRGB& p2, fract8 amountOfP2 );

CHSV  blend( const CHSV& p1, const CHSV& p2, fract8 amountOfP2,
            TGradientDirectionCode directionCode = SHORTEST_HUES );

// blend - computes a new color blended array of colors, each
//         a given fraction of the way between corresponding
//         elements of two source arrays of colors.
//         Useful for blending palettes.
CRGB* blend( const CRGB* src1, const CRGB* src2, CRGB* dest,
            uint16_t count, fract8 amountOfsrc2 );

CHSV* blend( const CHSV* src1, const CHSV* src2, CHSV* dest,
            uint16_t count, fract8 amountOfsrc2,
            TGradientDirectionCode directionCode = SHORTEST_HUES );

// nblend - destructively modifies one color, blending
//          in a given fraction of an overlay color
CRGB& nblend( CRGB& existing, const CRGB& overlay, fract8 amountOfOverlay );

CHSV& nblend( CHSV& existing, const CHSV& overlay, fract8 amountOfOverlay,
             TGradientDirectionCode directionCode = SHORTEST_HUES );

// nblend - destructively blends a given fraction of
//          a new color array into an existing color array
void  nblend( CRGB* existing, CRGB* overlay, uint16_t count, fract8 amountOfOverlay);

void  nblend( CHSV* existing, CHSV* overlay, uint16_t count, fract8 amountOfOverlay,
             TGradientDirectionCode directionCode = SHORTEST_HUES);


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
void blur1d( CRGB* leds, uint16_t numLeds, fract8 blur_amount);
void blur2d( CRGB* leds, uint8_t width, uint8_t height, fract8 blur_amount);

// blurRows: perform a blur1d on every row of a rectangular matrix
void blurRows( CRGB* leds, uint8_t width, uint8_t height, fract8 blur_amount);
// blurColumns: perform a blur1d on each column of a rectangular matrix
void blurColumns(CRGB* leds, uint8_t width, uint8_t height, fract8 blur_amount);

typedef enum { NOBLEND=0, LINEARBLEND=1 } TBlendType;


///@}

#endif /* colorutils_h */
