#include <SLIPEncodedUSBSerial.h>
#include <OSCMessage.h>

const int BUTTON0 = 6; // joystick push button (does not need resistor)
const int HORIZ = 0; // horiz axis
const int VERT = 1; // vert axis
const int BUZZ = 4; // buzzer - wants PWM
const int RANGE = 3; // analog output of ultrasonic range finder

int button0 = 0;
int horiz = 0;
int vert = 0;
int range = 0;

// teensy has serial USB :)
SLIPEncodedUSBSerial SLIPSerial( thisBoardsSerialUSB );

void setup() {
  pinMode(BUTTON0, INPUT);
  digitalWrite(BUTTON0, HIGH);
  
  SLIPSerial.begin(250000);
}

void sendOSCMessage(const char * message, int value) {
  OSCMessage msg(message);
  msg.add((int32_t)value);

  SLIPSerial.beginPacket();  
  msg.send(SLIPSerial); 
  SLIPSerial.endPacket(); 
  msg.empty(); 
}

void loop() {
  // == SEND ==
  // JOYSTICK - HORIZ
  int nHoriz = analogRead(HORIZ);
  if (abs(horiz - nHoriz) > 2) {
    horiz = nHoriz;
    sendOSCMessage("/joystick/horiz", horiz);
  }
  
  // JOYSTICK - VERT
  int nVert = analogRead(VERT);
  if (abs(vert - nVert) > 2) {
    vert = nVert;
    sendOSCMessage("/joystick/vert", vert);
  }

  // BUTTON0
  int nButton0 = digitalRead(BUTTON0);
  if (button0 != nButton0) {
    button0 = nButton0;
    sendOSCMessage("/button/0", button0);
  }

  // RANGE
  // TODO: output is wild - NOT WORKING
  int nRange = analogRead(RANGE);
  if (range != nRange) {
    range = nRange;
    sendOSCMessage("/range", range);
  }





  // == RECEIVE ==
  /*
  OSCBundle bundleIN;
  int size;

  while(!SLIPSerial.endofPacket())
    if( (size =SLIPSerial.available()) > 0)
    {
       while(size--)
          bundleIN.fill(SLIPSerial.read());
     }
  
  if(!bundleIN.hasError())
   bundleIN.dispatch("/led", LEDcontrol);
*/


   



  // throttle updates so we don't choke receiver
  delay(20);
}
