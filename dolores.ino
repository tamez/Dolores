/*********************************************************************
This is an example for our nRF8001 Bluetooth Low Energy Breakout

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1697

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Kevin Townsend/KTOWN  for Adafruit Industries.
MIT license, check LICENSE for more information
All text above, and the splash screen below must be included in any redistribution
*********************************************************************/

// This version uses the internal data queing so you can treat it like Serial (kinda)!

#include <SPI.h>
#include "Adafruit_BLE_UART.h"

// Connect CLK/MISO/MOSI to hardware SPI
// Here we define the pins for BLE #1
#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 3     // This should be an interrupt pin, on Uno thats #2 or #3
#define ADAFRUITBLE_RST 9

// Then we define the seperate pins for BLE #2
#define ADAFRUITBLE_REQ02 4
#define ADAFRUITBLE_RDY02 2     // This should be an interrupt pin, on Uno thats #2 or #3
#define ADAFRUITBLE_RST02 5

#define ledPin 13

// The first object was defined for us. So, now we define a second object. One for each BLE
Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
Adafruit_BLE_UART BTLEserial02 = Adafruit_BLE_UART(ADAFRUITBLE_REQ02, ADAFRUITBLE_RDY02, ADAFRUITBLE_RST02);

/**************************************************************************/
/*!
    Configure the Arduino and start advertising with the radio
*/
/**************************************************************************/
void setup(void)
{ 
  Serial.begin(9600);
  while(!Serial); // Leonardo/Micro should wait for serial init
  Serial.println(F("Welcome to the Dolores Project!"));

  BTLEserial.begin();
}

/**************************************************************************/
/*!
    Constantly checks for new events on the nRF8001
*/
/**************************************************************************/
aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;
aci_evt_opcode_t laststatus2 = ACI_EVT_DISCONNECTED;

void loop()
{
  // Tell the nRF8001 to do whatever it should be working on.
  BTLEserial.pollACI();

  // Ask what is our current status
  aci_evt_opcode_t status = BTLEserial.getState();
  // If the status changed....
  if (status != laststatus) {
    // print it out!
    if (status == ACI_EVT_DEVICE_STARTED) {
        Serial.println(F("* BLE ONE is advertising"));
        digitalWrite(ledPin, HIGH);
    }
    if (status == ACI_EVT_CONNECTED) {
        Serial.println(F("* Connected!"));
    }
    if (status == ACI_EVT_DISCONNECTED) {
        Serial.println(F("* Disconnected or advertising timed out"));
    }
    // OK set the last status change to this one
    laststatus = status;
  }

  if (status == ACI_EVT_CONNECTED) {
    // Lets see if there's any data for us!
    if (BTLEserial.available()) {
      Serial.print("* "); Serial.print(BTLEserial.available()); Serial.println(F(" bytes available from BTLE"));
    }
    // OK while we still have something to read, get a character and print it out
    while (BTLEserial.available()) {
      char c = BTLEserial.read();
      Serial.print(c);
    }

    // Next up, see if we have any data to get from the Serial console

    if (Serial.available()) {
      // Read a line from Serial
      Serial.setTimeout(100); // 100 millisecond timeout
      String s = Serial.readString();

      // We need to convert the line to bytes, no more than 20 at this time
      uint8_t sendbuffer[20];
      s.getBytes(sendbuffer, 20);
      char sendbuffersize = min(20, s.length());

      Serial.print(F("\n* Sending -> \"")); Serial.print((char *)sendbuffer); Serial.println("\"");

      // write the data
      BTLEserial.write(sendbuffer, sendbuffersize);
    }
  }

/**************************************************************************/
/*!
    Repeat for BLE 02 & Constantly checks for new events on the nRF8001
*/
/**************************************************************************/


  // Tell the nRF8001 to do whatever it should be working on.
  BTLEserial02.pollACI();

  // Ask what is our current status
  aci_evt_opcode_t status2 = BTLEserial02.getState();
  
  // If the status changed....
  if (status2 != laststatus) {
    // print it out!
    if (status2 == ACI_EVT_DEVICE_STARTED) {
        Serial.println(F("* BLE2 is advertising"));
        digitalWrite(ledPin, HIGH);
    }
    if (status2 == ACI_EVT_CONNECTED) {
        Serial.println(F("* Connected!"));
    }
    if (status2 == ACI_EVT_DISCONNECTED) {
        Serial.println(F("* Disconnected or advertising timed out"));
    }
    // OK set the last status change to this one
    laststatus2 = status2;
  }

  if (status2 == ACI_EVT_CONNECTED) {
    // Lets see if there's any data for us!
    if (BTLEserial02.available()) {
      Serial.print("* "); Serial.print(BTLEserial02.available()); Serial.println(F(" bytes available from BTLE"));
    }
    // OK while we still have something to read, get a character and print it out
    while (BTLEserial02.available()) {
      char c = BTLEserial02.read();
      Serial.print(c);
    }

    // Next up, see if we have any data to get from the Serial console

    if (Serial.available()) {
      // Read a line from Serial
      Serial.setTimeout(100); // 100 millisecond timeout
      String s = Serial.readString();

      // We need to convert the line to bytes, no more than 20 at this time
      uint8_t sendbuffer[20];
      s.getBytes(sendbuffer, 20);
      char sendbuffersize = min(20, s.length());

      Serial.print(F("\n* Sending -> \"")); Serial.print((char *)sendbuffer); Serial.println("\"");

      // write the data
      BTLEserial02.write(sendbuffer, sendbuffersize);
    }
  }
  
}


