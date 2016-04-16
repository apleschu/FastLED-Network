//
//  FastledDefinitions.h
//  FastLED
//
//  Created by Andreas Pleschutznig on 4/7/16.
//  Copyright © 2016 Andreas Pleschutznig. All rights reserved.
//

#ifndef FastledDefinitions_h
#define FastledDefinitions_h

#define PHASE1                  1           // Encoded the whole stream
#define PHASE2                  2           // we encoded the CRGB[]
#define UNCOMPRESSED            3
#define FastledShow             5           // Execute the show()
#define FastledSetBrightness    6
#define FastledSetNumLeds       11

#define SYN                     0x16
#define SOH                     0x01
#define STX                     0x02
#define ETX                     0x03
#define DELIM                   0x20


#endif /* FastledDefinitions_h */
