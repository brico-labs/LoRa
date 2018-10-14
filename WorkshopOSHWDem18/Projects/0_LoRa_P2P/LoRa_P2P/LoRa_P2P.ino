/*
  LoRa Duplex communication wth callback based on Tom Igoe example
  Modified by Alex TC @TCRobotics for 1 BricoLabs ESP8266 LoRa shield

  Sends a message every half second, and uses callback
  for new incoming messages. Implements a one-byte addressing scheme,
  with broadcast address.

  Note: while sending, LoRa radio is not listening for incoming messages.
  Note2: when using the callback method, you can't use any of the Stream
  functions that rely on the timeout, such as readString, parseInt(), etc.
*/
#include <Arduino.h>
#include <SPI.h>              // include libraries
#include <LoRa.h>    //https://github.com/sandeepmistry/arduino-LoRa

#define RFM95W_SCK  D5 // GPIO14 -- RFM95W's SCK
#define RFM95W_MISO D6 // GPIO12 -- RFM95W's MISO
#define RFM95W_MOSI D7 // GPIO13 -- RFM95W's MOSI
#define RFM95W_CS   D8 // GPIO15 -- RFM95W's CS
#define RFM95W_RST  D4 // GPIO2  -- RFM95W's RESET NOT USED! Pin not connected to ESP8266
#define RFM95W_DI0  D3 // GPIO0  -- RFM95W's IRQ(Interrupt Request) NOT USED! Pin not connected to ESP8266

#define LORA_BAND   868E6

//////////////////////CONFIG///////////////////////////
byte localAddress = XX;     // address of this device
byte destination  = XX;      // destination to send to
String message    = "Hello!";   // send a message
///////////////////////////////////////////////////////

int interval = 2000;          // interval between sends
String outgoing;              // outgoing message
byte msgCount = 0;            // count of outgoing messages
long lastSendTime = 0;        // last send time

void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  delay(1000);
  msgCount++;                           // increment message ID
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";                 // payload of packet

  while (LoRa.available()) {            // can't use readString() in callback, so
    incoming += (char)LoRa.read();      // add bytes one by one
  }

  if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    incoming = "message length error";
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    //Serial.println("This message is not for me.");
    //incoming = "message is not for me";
    return;                             // skip rest of function
  }
  
  // if message is for this device, or broadcast, print details:
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
  delay(1000);
}

void setup() {
  Serial.begin(115200);                   // initialize serial
  //while (!Serial);
  delay(1000);
  Serial.println("LoRa Duplex sender");

  //SPI.begin(SX1278_SCK, SX1278_MISO, SX1278_MOSI, SX1278_CS);
  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(RFM95W_CS, RFM95W_RST, RFM95W_DI0);// set CS, reset, IRQ pin

  if (!LoRa.begin(LORA_BAND))
  {             // initialize ratio at 868 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true){}                       // if failed, do nothing
  }

  LoRa.onReceive(onReceive);
  //LoRa.receive();
  Serial.println("LoRa init succeeded.");
}

void loop() {
  if (millis() - lastSendTime > interval) {
    sendMessage(message);
    Serial.println("Sending " + message);
    lastSendTime = millis();            // timestamp the message
    interval = random(2000) + 1000;     // 2-3 seconds
    LoRa.receive();                     // go back into receive mode
  }
  yield();
  int packetSize = LoRa.parsePacket();
  if (packetSize) { onReceive(packetSize);  }
}
