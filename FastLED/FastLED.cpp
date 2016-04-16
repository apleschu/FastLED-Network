//
//  FastLED.cpp
//  FastLED
//
//  Created by Andreas Pleschutznig on 4/14/16.
//  Copyright © 2016 Andreas Pleschutznig. All rights reserved.
//

#include <stdio.h>
#include <string.h>         //strlen
#include <sys/socket.h>     //socket
#include <arpa/inet.h>      //inet_addr
#include <unistd.h>         // sleep functions
#include <netinet/tcp.h>

#include "FastLED.h"
#include "FastledDefinitions.h"

/***************************************************************************
 *   Function   : Encode
 *   Description: This routine reads an input string and writes out a run
 *                length encoded version of that string.
 *   Parameters : inFile - Pointer to the char[] to encode
 *                outFile - Pointer to the char[] to write encoded output to
 *   Effects    : File is encoded using RLE
 *   Returned   : 0 for success, -1 for failure.
 ***************************************************************************/

#define escChar 0xff
#define UCHAR_MAX 250       // max run length to encode

int delay(uint16_t delayTime){
    useconds_t sleepTime = delayTime * 1000;
    return(usleep(sleepTime));
}

int RleEncodePass1(unsigned char *inFile, unsigned int InLength, unsigned char *outFile, unsigned int *OutLength)
{
    unsigned char currChar;                       /* current characters */
    unsigned char prevChar;                       /* previous characters */
    unsigned char count;                          /* number of characters in a run */
    int i;                                        /* Counter */
    int outCount;
    
    /* validate input and output files */
    if ((NULL == inFile) || (NULL == outFile))
    {
        return -1;
    }
    
    /* encode inFile */
    prevChar = escChar;     /* force next char to be different */
    count = 0;
    
    outCount = 0;
    /* read input until there's nothing left */
    for ( i = 0; i < InLength; i++)
    {
        currChar = inFile[i];                   // read next char from stream
        outFile[outCount++] = currChar;
        
        /* check for run */
        if (currChar == prevChar)
        {
            /* we have a run.  count run length */
            count = 0;
            
            while (i < InLength)
            {
                currChar = inFile[i++];
                if (currChar == prevChar)
                {
                    count++;
                    
                    if (count == UCHAR_MAX)
                    {
                        /* count is as long as it can get */
                        outFile[outCount++] = count;
                        
                        /* force next char to be different */
                        prevChar = EOF;
                        break;
                    }
                }
                else
                {
                    /* run ended */
                    outFile[outCount++] = count;
                    outFile[outCount++] = currChar;
                    prevChar = currChar;
                    break;
                }
            }
        }
        else
        {
            /* no run */
            prevChar = currChar;
        }
        
        if (i == (InLength))
        {
            /* run ended because of EOF */
            outFile[outCount++] = count;
            break;
        }
    }
    
    *OutLength = outCount;
    return 0;
}

struct RGBVal {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

// Pass #2 encodes color triplets
int RleEncodePass2(unsigned char *inFile, unsigned int InLength, unsigned char *outFile, unsigned int *OutLength)
{
    unsigned char currRed, currGreen, currBlue;                       /* current characters */
    unsigned char prevRed, prevGreen, prevBlue;                       /* previous characters */
    unsigned char count;                          /* number of characters in a run */
    int i;                                        /* Counter */
    int outCount;
    
    // we have to be careful here only to work on valid streams. in this case since the stream is a CRGB[]
    // the stream has to be in multiples of 3
    if ((InLength % 3) ) {
        *OutLength = 9999;      // This should be longer than any strip we are working with
        return(-1);             // error
    }
    
    /* validate input and output files */
    if ((NULL == inFile) || (NULL == outFile))
    {
        return -1;
    }
    
    /* encode inFile */
    prevRed = 0;
    prevGreen = EOF;
    prevBlue = 128;     /* force next char to be different */
    count = 0;
    
    outCount = 0;
    /* read input until there's nothing left */
    for ( i = 0; i < InLength; )
    {
        currRed = inFile[i++];                   // read next char from stream
        currGreen = inFile[i++];
        currBlue = inFile[i++];
        outFile[outCount++] = currRed;
        outFile[outCount++] = currGreen;
        outFile[outCount++] = currBlue;
        
        /* check for run */
        if (currRed == prevRed && currGreen == prevGreen && currBlue == prevBlue)
        {
            /* we have a run.  count run length */
            count = 0;
            
            while (i < InLength)
            {
                currRed = inFile[i++];
                currGreen = inFile[i++];
                currBlue = inFile[i++];
                
                if (currRed == prevRed && currGreen == prevGreen && currBlue == prevBlue)
                {
                    count++;
                    
                    if (count == UCHAR_MAX)
                    {
                        /* count is as long as it can get */
                        outFile[outCount++] = count;
                        
                        /* force next char to be different */
                        prevRed = 0;
                        prevGreen = EOF;
                        prevBlue = 128;
                        break;
                    }
                }
                else
                {
                    /* run ended */
                    outFile[outCount++] = count;
                    outFile[outCount++] = currRed;
                    outFile[outCount++] = currGreen;
                    outFile[outCount++] = currBlue;
                    prevRed = currRed;
                    prevGreen = currGreen;
                    prevBlue = currBlue;
                    break;
                }
            }
        }
        else
        {
            /* no run */
            prevRed = currRed;
            prevGreen = currGreen;
            prevBlue = currBlue;
        }
        
        if (i == (InLength))
        {
            /* run ended because of EOF */
            outFile[outCount++] = count;
            break;
        }
    }
    
    *OutLength = outCount;
    return 0;
}

int connect8266(char *ip, uint16_t port) {
    struct sockaddr_in server;
    
    //Create socket
    int sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
//    puts("Socket created");
    
    server.sin_addr.s_addr = inet_addr("10.0.1.11");
    server.sin_family = AF_INET;
    server.sin_port = htons( port );
    
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    int one = 1;
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    return(sock);
}
