#include <SPI.h>
#include "DW1000Ranging.h"
#include "DW1000Device.h"

// connection pins
const uint8_t PIN_RST = 9; // reset pin
const uint8_t PIN_IRQ = 2; // irq pin
const uint8_t PIN_SS = SS; // spi select pin

// DEBUG packet sent status and count
volatile boolean received = false;
volatile boolean error = false;
volatile int numReceived = 0;
String message;
volatile long counter = 0;

void handleReceived() {
  // status change on reception success
  received = true;
}

void receiver() {
  DW1000.newReceive();
  DW1000.setDefaults();
  // so we don't need to restart the receiver manually
  DW1000.startReceive();
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  //init the configuration
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); //Reset, CS, IRQ pin
  //define the sketch as anchor. It will be great to dynamically change the type of module
  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachBlinkDevice(newBlink);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);
  //Enable the filter to smooth the distance
  //DW1000Ranging.useRangeFilter(true);

  // attach callback for (successfully) received messages
  DW1000.attachReceivedHandler(handleReceived);
  DW1000.attachReceiveFailedHandler(handleError);
  DW1000.attachErrorHandler(handleError);

  //we start the module as an anchor
  //DW1000Ranging.startAsAnchor("82:17:5B:D5:A9:9A:E2:9C", DW1000.MODE_LONGDATA_RANGE_ACCURACY);
  DW1000Ranging.startAsAnchor("82:17:5B:D5:A9:9A:E2:9C", DW1000.MODE_LONGDATA_FAST_LOWPOWER);
}


void handleError() {
  error = true;
}


void loop() {

  counter ++;
  if (counter % 100 == 1)
  {
    DW1000Ranging.startAsAnchor("82:17:5B:D5:A9:9A:E2:8C", DW1000.MODE_LONGDATA_FAST_LOWPOWER);
  }
  if (counter % 100 > 1)
  {
    DW1000Ranging.loop();
    newRange();
  } else
  {
    receiver();
    // enter on confirmation of ISR status change (successfully received)
    if (received) {
      newRange();
      // get data as string
      DW1000.getData(message);
      Serial.print("Received message ... "); 
      Serial.println(message);
    }
  }
  delay(100);

}

void newRange() {
  delay(100);
  /*
  Serial.print("from: "); Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  Serial.print("\t Range: "); Serial.print(DW1000Ranging.getDistantDevice()->getRange()); Serial.print(" m");
  Serial.print("\t RX power: "); Serial.print(DW1000Ranging.getDistantDevice()->getRXPower()); Serial.println(" dBm");
  */
}

void newBlink(DW1000Device* device) {
  Serial.print("blink; 1 device added ! -> ");
  Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device* device) {
  Serial.print("delete inactive device: ");
  Serial.println(device->getShortAddress(), HEX);
}

