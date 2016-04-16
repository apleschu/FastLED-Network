//
//  scale8.h
//  FastLED
//
//  Created by Andreas Pleschutznig on 4/15/16.
//  Copyright © 2016 Andreas Pleschutznig. All rights reserved.
//

#ifndef scale8_h
#define scale8_h

///@ingroup lib8tion

///@defgroup Scaling Scaling functions
/// Fast, efficient 8-bit scaling functions specifically
/// designed for high-performance LED programming.
///
/// Because of the AVR(Arduino) and ARM assembly language
/// implementations provided, using these functions often
/// results in smaller and faster code than the equivalent
/// program using plain "C" arithmetic and logic.
///@{

///  scale one byte by a second one, which is treated as
///  the numerator of a fraction whose denominator is 256
///  In other words, it computes i * (scale / 256)
///  4 clocks AVR with MUL, 2 clocks ARM
LIB8STATIC uint8_t scale8( uint8_t i, fract8 scale)
{
    return ((uint16_t)i * (uint16_t)(scale) ) >> 8;
}


///  The "video" version of scale8 guarantees that the output will
///  be only be zero if one or both of the inputs are zero.  If both
///  inputs are non-zero, the output is guaranteed to be non-zero.
///  This makes for better 'video'/LED dimming, at the cost of
///  several additional cycles.
LIB8STATIC uint8_t scale8_video( uint8_t i, fract8 scale)
{
    uint8_t j = (((int)i * (int)scale) >> 8) + ((i&&scale)?1:0);
    // uint8_t nonzeroscale = (scale != 0) ? 1 : 0;
    // uint8_t j = (i == 0) ? 0 : (((int)i * (int)(scale) ) >> 8) + nonzeroscale;
    return j;
}


/// This version of scale8 does not clean up the R1 register on AVR
/// If you are doing several 'scale8's in a row, use this, and
/// then explicitly call cleanup_R1.
LIB8STATIC uint8_t scale8_LEAVING_R1_DIRTY( uint8_t i, fract8 scale)
{
    return ((int)i * (int)(scale) ) >> 8;
}

/// In place modifying version of scale8, also this version of nscale8 does not
/// clean up the R1 register on AVR
/// If you are doing several 'scale8's in a row, use this, and
/// then explicitly call cleanup_R1.

LIB8STATIC void nscale8_LEAVING_R1_DIRTY( uint8_t& i, fract8 scale)
{
    i = ((int)i * (int)(scale) ) >> 8;
}


/// This version of scale8_video does not clean up the R1 register on AVR
/// If you are doing several 'scale8_video's in a row, use this, and
/// then explicitly call cleanup_R1.
LIB8STATIC uint8_t scale8_video_LEAVING_R1_DIRTY( uint8_t i, fract8 scale)
{
    uint8_t j = (((int)i * (int)scale) >> 8) + ((i&&scale)?1:0);
    // uint8_t nonzeroscale = (scale != 0) ? 1 : 0;
    // uint8_t j = (i == 0) ? 0 : (((int)i * (int)(scale) ) >> 8) + nonzeroscale;
    return j;
}

/// In place modifying version of scale8_video, also this version of nscale8_video
/// does not clean up the R1 register on AVR
/// If you are doing several 'scale8_video's in a row, use this, and
/// then explicitly call cleanup_R1.
LIB8STATIC void nscale8_video_LEAVING_R1_DIRTY( uint8_t & i, fract8 scale)
{
    i = (((int)i * (int)scale) >> 8) + ((i&&scale)?1:0);
}

/// Clean up the r1 register after a series of *LEAVING_R1_DIRTY calls
LIB8STATIC void cleanup_R1()
{
}


/// scale three one byte values by a fourth one, which is treated as
///         the numerator of a fraction whose demominator is 256
///         In other words, it computes r,g,b * (scale / 256)
///
///         THIS FUNCTION ALWAYS MODIFIES ITS ARGUMENTS IN PLACE

LIB8STATIC void nscale8x3( uint8_t& r, uint8_t& g, uint8_t& b, fract8 scale)
{
    r = ((int)r * (int)(scale) ) >> 8;
    g = ((int)g * (int)(scale) ) >> 8;
    b = ((int)b * (int)(scale) ) >> 8;
}

/// scale three one byte values by a fourth one, which is treated as
///         the numerator of a fraction whose demominator is 256
///         In other words, it computes r,g,b * (scale / 256), ensuring
/// that non-zero values passed in remain non zero, no matter how low the scale
/// argument.
///
///         THIS FUNCTION ALWAYS MODIFIES ITS ARGUMENTS IN PLACE
LIB8STATIC void nscale8x3_video( uint8_t& r, uint8_t& g, uint8_t& b, fract8 scale)
{
    uint8_t nonzeroscale = (scale != 0) ? 1 : 0;
    r = (r == 0) ? 0 : (((int)r * (int)(scale) ) >> 8) + nonzeroscale;
    g = (g == 0) ? 0 : (((int)g * (int)(scale) ) >> 8) + nonzeroscale;
    b = (b == 0) ? 0 : (((int)b * (int)(scale) ) >> 8) + nonzeroscale;
}

///  scale two one byte values by a third one, which is treated as
///         the numerator of a fraction whose demominator is 256
///         In other words, it computes i,j * (scale / 256)
///
///         THIS FUNCTION ALWAYS MODIFIES ITS ARGUMENTS IN PLACE

LIB8STATIC void nscale8x2( uint8_t& i, uint8_t& j, fract8 scale)
{
    i = ((int)i * (int)(scale) ) >> 8;
    j = ((int)j * (int)(scale) ) >> 8;
}

///  scale two one byte values by a third one, which is treated as
///         the numerator of a fraction whose demominator is 256
///         In other words, it computes i,j * (scale / 256), ensuring
/// that non-zero values passed in remain non zero, no matter how low the scale
/// argument.
///
///         THIS FUNCTION ALWAYS MODIFIES ITS ARGUMENTS IN PLACE


LIB8STATIC void nscale8x2_video( uint8_t& i, uint8_t& j, fract8 scale)
{
    uint8_t nonzeroscale = (scale != 0) ? 1 : 0;
    i = (i == 0) ? 0 : (((int)i * (int)(scale) ) >> 8) + nonzeroscale;
    j = (j == 0) ? 0 : (((int)j * (int)(scale) ) >> 8) + nonzeroscale;
}


/// scale a 16-bit unsigned value by an 8-bit value,
///         considered as numerator of a fraction whose denominator
///         is 256. In other words, it computes i * (scale / 256)

LIB8STATIC uint16_t scale16by8( uint16_t i, fract8 scale )
{
    uint16_t result;
    result = (i * scale) / 256;
    return result;
}

/// scale a 16-bit unsigned value by a 16-bit value,
///         considered as numerator of a fraction whose denominator
///         is 65536. In other words, it computes i * (scale / 65536)

LIB8STATIC uint16_t scale16( uint16_t i, fract16 scale )
{
    uint16_t result;
    result = ((uint32_t)(i) * (uint32_t)(scale)) / 65536;
    return result;
}
///@}

///@defgroup Dimming Dimming and brightening functions
///
/// Dimming and brightening functions
///
/// The eye does not respond in a linear way to light.
/// High speed PWM'd LEDs at 50% duty cycle appear far
/// brighter then the 'half as bright' you might expect.
///
/// If you want your midpoint brightness leve (128) to
/// appear half as bright as 'full' brightness (255), you
/// have to apply a 'dimming function'.
///@{

/// Adjust a scaling value for dimming
LIB8STATIC uint8_t dim8_raw( uint8_t x)
{
    return scale8( x, x);
}

/// Adjust a scaling value for dimming for video (value will never go below 1)
LIB8STATIC uint8_t dim8_video( uint8_t x)
{
    return scale8_video( x, x);
}

/// Linear version of the dimming function that halves for values < 128
LIB8STATIC uint8_t dim8_lin( uint8_t x )
{
    if( x & 0x80 ) {
        x = scale8( x, x);
    } else {
        x += 1;
        x /= 2;
    }
    return x;
}

/// inverse of the dimming function, brighten a value
LIB8STATIC uint8_t brighten8_raw( uint8_t x)
{
    uint8_t ix = 255 - x;
    return 255 - scale8( ix, ix);
}

/// inverse of the dimming function, brighten a value
LIB8STATIC uint8_t brighten8_video( uint8_t x)
{
    uint8_t ix = 255 - x;
    return 255 - scale8_video( ix, ix);
}

/// inverse of the dimming function, brighten a value
LIB8STATIC uint8_t brighten8_lin( uint8_t x )
{
    uint8_t ix = 255 - x;
    if( ix & 0x80 ) {
        ix = scale8( ix, ix);
    } else {
        ix += 1;
        ix /= 2;
    }
    return 255 - ix;
}

///@}


#endif /* scale8_h */
