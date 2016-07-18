
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
  
  //Init the configuration
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); //Reset, CS, IRQ pin
  
  // Define the sketch as anchor
  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachNewDevice(newDevice);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);

  // We start the module as a tag, paramters are the tag's address and the mode of the chip
  DW1000Ranging.startAsTag("7D:00:22:EA:82:60:3B:9C", DW1000.MODE_LONGDATA_FAST_LOWPOWER);
}

void loop() {

  // Loop the ranging 
  DW1000Ranging.loop();
}

void newRange() {

  // Output for ranging, can be modified to suit our needs
  Serial.print("from: "); Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  Serial.print("\t Range: "); Serial.print(DW1000Ranging.getDistantDevice()->getRange()); Serial.print(" m");
  Serial.print("\t RX power: "); Serial.print(DW1000Ranging.getDistantDevice()->getRXPower()); Serial.println(" dBm");
}

void newDevice(DW1000Device* device) {

  // Output the detection of a device
  Serial.print("ranging init; 1 device added ! -> ");
  Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device* device) {

   // Output the disconnection of a device
  Serial.print("delete inactive device: ");
  Serial.println(device->getShortAddress(), HEX);
}
