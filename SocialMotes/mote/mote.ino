#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>


/* Status LED */
const int STATUS_LED = 5;

/* Wifi */
const char* ssid = "TRUMP 4 PREZ";
const char* password = "monkeybusine$$";

/* UDP */
// nNeed to assign static IP to master control
byte UDP_REMOTE_IP[] = { 192, 168, 0, 11 };
int UDP_PORT = 2390;
WiFiUDP udpConn;

/* Pixels */
const int NEO_PIXEL_PIN = 4; // D4 is pixel pin
const int NEO_PIXEL_COUNT = 5;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ400);

/* Checking */
const int LOOP_SLEEP = 20;
int checkInDelay = 0;
const int checkInReset = 500; // 500 * 20 = 10 seconds


void setup() {
  Serial.begin(9600);

  // status LED
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW);

  // boot up wifi
  beginNetwork();

  // init IMU
  beginIMU();
  
  // initialze NeoPixels
  pixels.begin();
  for(int i = 0; i < NEO_PIXEL_COUNT; i++)
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  pixels.show();
}

void loop() {
  // incoming
  OSCBundle msgIn;
  int size;
  if( (size = udpConn.parsePacket()) > 0) {
    while(size--)
      msgIn.fill(udpConn.read());

    if (!msgIn.hasError()) {      
      msgIn.route("/led", routeLed);
    } else {
      Serial.println("OSC MESSAGE ERROR");
      Serial.println(msgIn.getError());
    }
  }
  

  delay(LOOP_SLEEP); // allow ESP8266 to do some work in the background
  // TODO: don't use loop - switch to interrupts
  // can we interrupt on incoming OSC?


  // check in with master control ever so often
  if (checkInDelay++ == 0)
    sayHi();
  if (checkInDelay > checkInReset)
    checkInDelay = 0;
}


// === OSC ===
void sendOSCMessage(OSCMessage msg) {
  udpConn.beginPacket(UDP_REMOTE_IP, UDP_PORT);
  msg.send(udpConn); 
  udpConn.endPacket();

  msg.empty(); 
}

void sayHi() {
  // check in with master control
  char myIpString[24];
  IPAddress myIp = WiFi.localIP();
  sprintf(myIpString, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);

  OSCMessage msg("/hi");
  msg.add(myIpString);

  // read ADC for current batt voltage
  int battVcc = analogRead(A0);
  Serial.println("BATT VCC:");
  Serial.println(battVcc);
  msg.add((int32_t)battVcc);

  sendOSCMessage(&msg);
}

//converts the pin to an osc address
char * numToOSCAddress( int pin){
    static char s[10];
    int i = 9;
  
    s[i--]= '\0';
  do
    {
    s[i] = "0123456789"[pin % 10];
                --i;
                pin /= 10;
    }
    while(pin && i);
    s[i] = '/';
    return &s[i];
}


// === ROUTES ===
/*
 * /led/0 R(int) G(int) B(int)
 */
void routeLed(OSCMessage &msg, int addrOffset) {  
  for(byte led = 0; led < NEO_PIXEL_COUNT; led++) {
    int ledMatched = msg.match(numToOSCAddress(led), addrOffset);
    if(ledMatched) {

      int r = msg.getInt(0);
      int g = msg.getInt(1);
      int b = msg.getInt(2);
      
      pixels.setPixelColor(led, pixels.Color(r, g, b));
      pixels.show();
    }
  }
}


// === NETWORK ===
void beginNetwork() {
    byte ledStatus = LOW;
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA); // connect to local wifi, don't make an access point
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    // Blink the LED
    digitalWrite(STATUS_LED, ledStatus); // Write LED high/low
    ledStatus = (ledStatus == HIGH) ? LOW : HIGH;
    
    // blocking loops will cause SOC to reset
    delay(100);
    Serial.print(".");
  }
  udpConn.begin(UDP_PORT);
  digitalWrite(STATUS_LED, HIGH);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


// === IMU ===
void beginIMU() {
}

