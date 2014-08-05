/* The Dolores Project
 * Last Updated: 7.19.14
 * Authors: Danny Tamez and Derek Redfern
 * License: All rights reserved.
 *
 * Summary:
 */

#include <SoftwareSerial.h>

// BLE0 will be on the hardware serial port, while BLE1 will be on a 
//  software serial port. Note that this means that you cannot use the
//  Arduino serial console for debugging!
// If we need to, it may be possible to create 2 Software serial ports
//  if we are very careful and make the appropriate sacrifices to the C gods.

                                // BLE#   RX   TX   VIN   BLE ROLE      CONNECTED TO
#define ble0 Serial             // 0      0    1    5V    PERIPHERAL    PHONE
SoftwareSerial ble1(10, 11);    // 1      10   11   4     PERIPHERAL    OTHER DEVICE

#define btnPin        12                // user input from physical button
#define ledPin        13                // LED debug pin
#define idNum         "TEST ID NUMBER"  // ID number to send to a remote phone
#define ble1_vin_pin  4                 // used to turn the 2nd BLE chip on/off

int buttonVal; // set whenever the debounced button value changes
int lastButtonVal; // remembers what the undebounced button value was last loop
unsigned long debounce; // remembers when the lastButtonVal changed last
int bleAdvertising;

void setup(void)
{
  ble0.begin(57600);
  ble1.begin(57600);

  pinMode(btnPin, INPUT);
  pinMode(ledPin, OUTPUT); 
  pinMode(ble1_vin_pin, OUTPUT);

  digitalWrite(ledPin,LOW);
  digitalWrite(ble1_vin_pin,LOW);

  buttonVal = LOW;
  lastButtonVal = LOW;
  debounce = -1;
  bleAdvertising = false;
}

void loop()
{
  /* BUTTON DEBOUNCING */
  int buttonRead = digitalRead(btnPin);

  if (lastButtonVal != buttonRead) {
    debounce = millis();
  }

  if (millis() - debounce > 100 && buttonRead != buttonVal) {
    buttonVal = buttonRead;

    if (buttonVal==HIGH) {
      // let the phone know that the button was pressed so it can look for devices
      ble0.write('A');

      // activate the second BLE chip
      digitalWrite(ble1_vin_pin,HIGH);
      bleAdvertising = true;
    }
  }

  /* HANDLING REQUESTS FROM REMOTE PHONE */
  if (bleAdvertising) {
    if (ble1.available()) {
      if (ble1.read()=='B') {
        // another phone wants profile data; send it our ID number for lookup in the cloud
        ble1.println(idNum);
      }
    }
  }
}