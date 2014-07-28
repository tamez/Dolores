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
#define ble0 Serial             // 0      0    1    4     PERIPHERAL    PHONE
SoftwareSerial ble1(10, 11);    // 1      10   11   5     PERIPHERAL    OTHER DEVICE

#define btnPin -1 // what pin is this on again?
#define ledPin 13

void setup(void)
{ 
  // We can support higher speeds than 57600, but this is what's preprogrammed
  //  on the TI BLE chip.
  ble0.begin(57600);
  ble1.begin(57600);
}

void loop()
{
  // For now, there's nothing here. 
  // Oh well.
}