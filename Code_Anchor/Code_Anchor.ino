#include <SPI.h>
#include "DW1000Ranging.h"
#include "DW1000Device.h"

// connection pins
const uint8_t PIN_RST = 9; // reset pin
const uint8_t PIN_IRQ = 2; // irq pin
const uint8_t PIN_SS = SS; // spi select pin

void setup() {

  // Parameter = Baud rate
  Serial.begin(9600);
  delay(1000);
  
  // Init the configuration
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); //Reset, CS, IRQ pin
  
  // Define the sketch as anchor
  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachBlinkDevice(newBlink);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);

  // We start the module as an anchor, parameters are the anchor's address and the mode of the chip
  DW1000Ranging.startAsAnchor("82:17:5B:D5:A9:9A:E2:9C", DW1000.MODE_LONGDATA_FAST_LOWPOWER);
}

void loop() {

  // Loop the ranging 
  DW1000Ranging.loop();
}

void newRange() {

  // For calibrating the range
  double currRange = DW1000Ranging.getDistantDevice()->getRange();

  // Invert negative range
  if (currRange < 0)
  {
    currRange = -currRange;
  }
  
  // Output for ranging, can be modified to suit our needs
  Serial.print("from: "); Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  Serial.print("\t Range: "); Serial.print(currRange); Serial.print(" m");
  Serial.print("\t RX power: "); Serial.print(DW1000Ranging.getDistantDevice()->getRXPower()); Serial.println(" dBm");
}

void newBlink(DW1000Device* device) {
  
  // Output the detection of a device
  Serial.print("blink; 1 device added ! -> ");
  Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device* device) {

  // Output the disconnection of a device
  Serial.print("Device disconnected: ");
  Serial.println(device->getShortAddress(), HEX);
}

