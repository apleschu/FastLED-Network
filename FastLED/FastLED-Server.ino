
#include <ESP8266WiFi.h>
#include <FastLED.h>

#include "FastledDefinitions.h"

#define DATA_PIN  12
#define CLOCK_PIN 14
CRGB leds[3000];              // define a led array long enough

const char* ssid = "FSM";
const char* password = "15Bananas";

WiFiServer server(0xfa57);
WiFiClient Client;

void decode1();
void decode2();

void setup() {
Serial.begin(115200);
WiFi.begin(ssid, password);
Serial.print("\nConnecting to "); Serial.println(ssid);
uint8_t i = 0;
while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
if (i == 21) {
Serial.print("Could not connect to"); Serial.println(ssid);
while (1) delay(500);
}
//start UART and the server
Serial.begin(115200);
server.begin();

Serial.print("Ready! ");
Serial.print(WiFi.localIP());
Serial.print(":");
Serial.print(0xfa57);
Serial.println("");
}

void loop() {
uint8_t i;
//check if there are any new clients
if (server.hasClient()) {
Client = server.available();
Serial.println("New connection");
Client.flush();


//no free/disconnected spot so reject
//WiFiClient Client = server.available();
//Client.stop();
}

//check client for data

while (Client && Client.connected()) {
uint8_t inChar;

Client.setNoDelay(true);
if (Client.available()) {
//get data from the director
// a valid command frame has the following base structure:
// SYN (0x16)
// SOH (0x01)
// STX (0x02)
// COMMAND (see Fastleddefinitions.h)
// parameter
// delimiter (0x20)
// parameter
// delimiter OR
// ETX (0x03)
while (Client.available()) {
char buffer[10];
inChar = Client.read();
if (inChar == SYN) {
if (Client.read() == SOH) {
if (Client.read() == STX) {
// we have seen all three start chars, next character is the command
uint8_t command = Client.read();
switch (command) {
case UNCOMPRESSED:
{
uint16_t  messageLength = Client.read();
messageLength = messageLength << 8;
messageLength = messageLength + Client.read();
for (int i = 0; i < messageLength; i++) {
leds[i] = Client.read();
}
break;
}
case PHASE2:
{
uint8_t buffer[6000];
char b[80];
int i;
int index = 0;            // index into the leds[]

uint16_t  messageLength2 = Client.read();
messageLength2 = messageLength2 << 8;
messageLength2 = messageLength2 + Client.read();
for (i = 0; i < messageLength2; i++) {
buffer[i] = Client.read();
}

//                    for (int i = 0; i < messageLength2; i++) {
//                      sprintf(b, "%02X ", buffer[i]);
//                      Serial.print(b);
//                    }
//                    Serial.println("");

i = 0;
CRGB prev = CRGB(buffer[0]+1, buffer[1], buffer[2]);    // force the first pev to be different

while ( i < messageLength2 ) {
leds[index++] = CRGB(buffer[i], buffer[i+1], buffer[i+2]);
i += 3;
if (index > 1) {
if ( leds[index-1] == leds[index-2] )              // we have a run
{
CRGB prev = leds[index-1];

uint8_t runLength = buffer[i++];
for ( int y = 0; y < runLength; y++) {
leds[index++] = prev;
}
}
}
}
break;
}
case FastledSetBrightness:
{
uint8_t bright = Client.read();
FastLED.setBrightness(bright);
break;
}
case FastledShow:
{
FastLED.show();
break;
}
case FastledSetNumLeds:
{
uint16_t  NUM_LEDS = Client.read();
NUM_LEDS = NUM_LEDS << 8;
NUM_LEDS = NUM_LEDS + Client.read();
FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
}
default:
break;
}
}
}
}


}
}
}
// Client.stop()

//  if (Clients[0] && Clients[0].connected()) {
//    Clients[0].write("ACK", 3);
//    delay(1);
//  }

}