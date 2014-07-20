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
#define ble0 Serial // RX 0, TX 1
SoftwareSerial ble1(10, 11); // RX 10, TX 11

#define btnPin -1 // what pin is this on again?
#define ledPin 13

void setup(void)
{ 
  // We can support much higher speeds than 9600, but let's stick with this for now.
  ble0.begin(9600);
  ble1.begin(9600);
}

void loop()
{
  // For now, there's nothing here. 
  // Oh well.
}