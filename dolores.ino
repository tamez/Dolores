/* The Dolores Project
 * Last Updated: 7.19.14
 * Authors: Danny Tamez and Derek Redfern
 * License: All rights reserved.
 *
 * Summary:
 */

#include <SoftwareSerial.h>

// BLE0 will be on the hardware serial port, while BLE1 will be on a 
//  software serial port.

                                // BLE#   RX   TX   VIN   BLE ROLE      CONNECTED TO
#define ble0 Serial1            // 0      0    1    5V    PERIPHERAL    PHONE
SoftwareSerial ble1(10, 11);    // 1      10   11   4     PERIPHERAL    OTHER DEVICE

#define btnPin        12                // user input from physical button
#define ledPin        13                // LED debug pin
#define idNum         "TEST ID NUMBER"  // ID number to send to a remote phone
#define ble1_vin_pin  4                 // used to turn the 2nd BLE chip on/off
#define bleTimeout    10000     // milliseconds to advertise for

int buttonVal; // set whenever the debounced button value changes
int lastButtonVal; // remembers what the undebounced button value was last loop
unsigned long debounce; // remembers when the lastButtonVal changed last
int bleAdvertising;
unsigned long bleAdvertisingSince; // remembers how long we've been advertising for

void setup(void)
{
  ble0.begin(57600);
  ble1.begin(57600);
  Serial.begin(9600);

  pinMode(btnPin, INPUT);
  pinMode(ledPin, OUTPUT); 
  pinMode(ble1_vin_pin, OUTPUT);

  digitalWrite(ledPin,LOW);
  digitalWrite(ble1_vin_pin,HIGH);

  buttonVal = LOW;
  lastButtonVal = LOW;
  debounce = -1;
  bleAdvertising = false;
}

void loop()
{
  /* BUTTON DEBOUNCING */
  int buttonRead = digitalRead(btnPin);
  unsigned long now = millis();

  if (lastButtonVal != buttonRead) {
    debounce = now;
  }

  if (now - debounce > 100 && buttonRead != buttonVal) {
    buttonVal = buttonRead;

    if (buttonVal==HIGH) {
      // let the phone know that the button was pressed so it can look for devices
      // ble0.print("Advertising on");
      ble0.write('A');

      // activate the second BLE chip
      digitalWrite(ble1_vin_pin,LOW);
      bleAdvertising = true;
      bleAdvertisingSince = now;
    }
  }

  /* HANDLING REQUESTS FROM REMOTE PHONE */
  if (bleAdvertising) {
    if (ble1.available()) {
      char charRead = ble1.read(); // Seems to read garbled characters using SoftwareSerial
      if (charRead=='B') {
        // another phone wants profile data; send it our ID number for lookup in the cloud
        ble1.println(idNum);
      }
    }
  }

  if (bleAdvertising && (now - bleAdvertisingSince) > bleTimeout) {
    bleAdvertising = false;

    // deactivate the second BLE chip
    digitalWrite(ble1_vin_pin,HIGH);

    ble0.write('C');
  }

  lastButtonVal = buttonRead;
}