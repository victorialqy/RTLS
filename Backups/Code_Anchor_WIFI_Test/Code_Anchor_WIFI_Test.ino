//#include <WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266wifi.h>

#include <SPI.h>
#include "DW1000Ranging.h"
#include "DW1000Device.h"

#define sw_serial_rx_pin 4 //  Connect this pin to TX on the esp8266
#define sw_serial_tx_pin 6 //  Connect this pin to RX on the esp8266
#define esp8266_reset_pin 5 // Connect this pin to CH_PD on the esp8266, not reset. (let reset be unconnected)

// connection pins
const uint8_t PIN_RST = 9; // reset pin
const uint8_t PIN_IRQ = 2; // irq pin
const uint8_t PIN_SS = SS; // irq pin

String target = "SIT-WIFI";

#define SERVER_PORT "2121"
#define SSID "YourSSID"
#define PASSWORD "YourPassword"

SoftwareSerial swSerial(sw_serial_rx_pin, sw_serial_tx_pin);

// the last parameter sets the local echo option for the ESP8266 module..
ESP8266wifi wifi(Serial, Serial, esp8266_reset_pin, swSerial);


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
  //WiFi.connectToAP("VICTORIA.L","98524876");
    wifi.setTransportToTCP();
}

void loop() {

  // Loop the ranging 
  DW1000Ranging.loop();

//  boolean status = scanNetworks();
//  
//  // Wait a bit before scanning again
//  delay(500);
//  WifiMessage in = WiFi.listenForIncomingMessage(6000);
//    if (in.hasData) {
//        Serial.print("Incoming message:");
//        Serial.println(in.message);
//        if(in.channel == SERVER)
//            Serial.println("From server");
//        else{
//            Serial.print("From channel:");
//            Serial.println(in.channel);
//        }
//    }
//    // Do other stuff
}

//
//boolean scanNetworks() {
//  Serial.println("scan start");
//  boolean found = false;
//  // WiFi.scanNetworks will return the number of networks found
//  int n = WiFi.scanNetworks();
//  Serial.println("scan done");
//  if (n == 0)
//    Serial.println("no networks found");
//  else
//  {
//    Serial.print(n);
//    Serial.println(" networks found");
//    for (int i = 0; i < n; ++i)
//    {
//      if (target == WiFi.SSID(i)) {
//        found = true;
//        Serial.println("Target Found!");
//        Serial.print(i + 1);
//        Serial.print(": ");
//        Serial.print(WiFi.SSID(i));
//        Serial.print(" (");
//        Serial.print(WiFi.RSSI(i));
//        Serial.print(")");
//        Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
//      }
//    }
//  }
//  Serial.println("");
//  return found;
//}


void newRange() {

  // Output for ranging, can be modified to suit our needs
  Serial.print("from: "); Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  Serial.print("\t Range: "); Serial.print(DW1000Ranging.getDistantDevice()->getRange()); Serial.print(" m");
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

