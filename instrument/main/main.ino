#include <OSCMessage.h>
#include <SLIPEncodedUSBSerial.h>

SLIPEncodedUSBSerial SLIPSerial( thisBoardsSerialUSB );

void setup() {
  SLIPSerial.begin(9600);
}

void loop() {
  //the message wants an OSC address as first argument
  OSCMessage msg("/analog/0");
  msg.add((int32_t)analogRead(0));

  SLIPSerial.beginPacket();  
    msg.send(SLIPSerial); // send the bytes to the SLIP stream
  SLIPSerial.endPacket(); // mark the end of the OSC Packet
  msg.empty(); // free space occupied by message

  delay(200);
}
