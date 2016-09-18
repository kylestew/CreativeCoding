#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "quaternionFilters.h"
#include <MPU9250.h>


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

/* IMU */
#define AHRS false
#define SerialDebug false
MPU9250 imu;

/* Pixels */
const int NEO_PIXEL_PIN = 4; // D4 is pixel pin
const int NEO_PIXEL_COUNT = 5;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ400);
int r, g, b;

/* Checking */
const int LOOP_SLEEP = 10;
int checkInDelay = 0;
const int checkInReset = 1000; // 500 * 10 = 10 seconds


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
  r = g = b = 0;
  setAllPixels(r, g, b);
}

void loop() {
  // incoming OSC
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

  // imu
  updateIMU();



Serial.print(imu.ax);
Serial.print(" :: ");
Serial.print(imu.ay);
Serial.print(" :: ");
Serial.println(imu.az);

  // TEMP: output IMU data as R,G,B values
  r = lerp(r, (imu.ax + 1.0)/2.0 * 255, 0.2);
  g = lerp(g, (imu.ay + 1.0)/2.0 * 255, 0.2);
  b = lerp(b, (imu.az + 1.0)/2.0 * 255, 0.2);
  setAllPixels(r, g, b);



  delay(LOOP_SLEEP); // allow ESP8266 to do some work in the background
  // TODO: don't use loop - switch to interrupts
  // can we interrupt on incoming OSC?


  // check in with master control ever so often
  if (checkInDelay++ == 0)
    sayHi();
  if (checkInDelay > checkInReset)
    checkInDelay = 0;
}


// === PIXELS ===
void setAllPixels(int r, int g, int b) {
  for(int i = 0; i < NEO_PIXEL_COUNT; i++)
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  pixels.show();
}

int lerp(int start, int end, float percent) {
  return start + percent * (end - start);
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
  Wire.begin();

/*
    // Set up the interrupt pin, its set as active high, push-pull
  pinMode(intPin, INPUT);
  digitalWrite(intPin, LOW);
  pinMode(myLed, OUTPUT);
  digitalWrite(myLed, HIGH);
  */

  // Read the WHO_AM_I register, this is a good test of communication
  byte c = imu.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  Serial.print("MPU9250 "); Serial.print("I AM "); Serial.print(c, HEX);
  Serial.print(" I should be "); Serial.println(0x71, HEX);

      Serial.println("MPU9250 is online...");

    // Start by performing self test and reporting values
    imu.MPU9250SelfTest(imu.SelfTest);
    Serial.print("x-axis self test: acceleration trim within : ");
    Serial.print(imu.SelfTest[0],1); Serial.println("% of factory value");
    Serial.print("y-axis self test: acceleration trim within : ");
    Serial.print(imu.SelfTest[1],1); Serial.println("% of factory value");
    Serial.print("z-axis self test: acceleration trim within : ");
    Serial.print(imu.SelfTest[2],1); Serial.println("% of factory value");
    Serial.print("x-axis self test: gyration trim within : ");
    Serial.print(imu.SelfTest[3],1); Serial.println("% of factory value");
    Serial.print("y-axis self test: gyration trim within : ");
    Serial.print(imu.SelfTest[4],1); Serial.println("% of factory value");
    Serial.print("z-axis self test: gyration trim within : ");
    Serial.print(imu.SelfTest[5],1); Serial.println("% of factory value");

        // Calibrate gyro and accelerometers, load biases in bias registers
    imu.calibrateMPU9250(imu.gyroBias, imu.accelBias);


        imu.initMPU9250();
    // Initialize device for active mode read of acclerometer, gyroscope, and
    // temperature
    Serial.println("MPU9250 initialized for active data mode....");

    // Read the WHO_AM_I register of the magnetometer, this is a good test of
    // communication
    byte d = imu.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
    Serial.print("AK8963 "); Serial.print("I AM "); Serial.print(d, HEX);
    Serial.print(" I should be "); Serial.println(0x48, HEX);


    // Get magnetometer calibration from AK8963 ROM
    imu.initAK8963(imu.magCalibration);
    // Initialize device for active mode read of magnetometer
    Serial.println("AK8963 initialized for active data mode....");
    if (SerialDebug)
    {
      //  Serial.println("Calibration values: ");
      Serial.print("X-Axis sensitivity adjustment value ");
      Serial.println(imu.magCalibration[0], 2);
      Serial.print("Y-Axis sensitivity adjustment value ");
      Serial.println(imu.magCalibration[1], 2);
      Serial.print("Z-Axis sensitivity adjustment value ");
      Serial.println(imu.magCalibration[2], 2);
    }
}

void updateIMU() {
  if (imu.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01) {
    imu.readAccelData(imu.accelCount);  // Read the x/y/z adc values
    imu.getAres();
  
    // Now we'll calculate the accleration value into actual g's
    // This depends on scale being set
    imu.ax = (float)imu.accelCount[0]*imu.aRes; // - accelBias[0];
    imu.ay = (float)imu.accelCount[1]*imu.aRes; // - accelBias[1];
    imu.az = (float)imu.accelCount[2]*imu.aRes; // - accelBias[2];
  
    imu.readGyroData(imu.gyroCount);  // Read the x/y/z adc values
    imu.getGres();
  
    // Calculate the gyro value into actual degrees per second
    // This depends on scale being set
    imu.gx = (float)imu.gyroCount[0]*imu.gRes;
    imu.gy = (float)imu.gyroCount[1]*imu.gRes;
    imu.gz = (float)imu.gyroCount[2]*imu.gRes;
  
    imu.readMagData(imu.magCount);  // Read the x/y/z adc values
    imu.getMres();
    // User environmental x-axis correction in milliGauss, should be
    // automatically calculated
    imu.magbias[0] = +470.;
    // User environmental x-axis correction in milliGauss TODO axis??
    imu.magbias[1] = +120.;
    // User environmental x-axis correction in milliGauss
    imu.magbias[2] = +125.;
  
    // Calculate the magnetometer values in milliGauss
    // Include factory calibration per data sheet and user environmental
    // corrections
    // Get actual magnetometer value, this depends on scale being set
    imu.mx = (float)imu.magCount[0]*imu.mRes*imu.magCalibration[0] -
               imu.magbias[0];
    imu.my = (float)imu.magCount[1]*imu.mRes*imu.magCalibration[1] -
               imu.magbias[1];
    imu.mz = (float)imu.magCount[2]*imu.mRes*imu.magCalibration[2] -
               imu.magbias[2];
  }

  // Must be called before updating quaternions!
  imu.updateTime();

  // Sensors x (y)-axis of the accelerometer is aligned with the y (x)-axis of
  // the magnetometer; the magnetometer z-axis (+ down) is opposite to z-axis
  // (+ up) of accelerometer and gyro! We have to make some allowance for this
  // orientationmismatch in feeding the output to the quaternion filter. For the
  // MPU-9250, we have chosen a magnetic rotation that keeps the sensor forward
  // along the x-axis just like in the LSM9DS0 sensor. This rotation can be
  // modified to allow any convenient orientation convention. This is ok by
  // aircraft orientation standards! Pass gyro rate as rad/s
  MahonyQuaternionUpdate(imu.ax, imu.ay, imu.az, imu.gx*DEG_TO_RAD,
                         imu.gy*DEG_TO_RAD, imu.gz*DEG_TO_RAD, imu.my,
                         imu.mx, imu.mz, imu.deltat);

  if (!AHRS) {
    imu.delt_t = millis() - imu.count;
    if (imu.delt_t > 500) {
      if(SerialDebug)
      {
        // Print acceleration values in milligs!
        Serial.print("X-acceleration: "); Serial.print(1000*imu.ax);
        Serial.print(" mg ");
        Serial.print("Y-acceleration: "); Serial.print(1000*imu.ay);
        Serial.print(" mg ");
        Serial.print("Z-acceleration: "); Serial.print(1000*imu.az);
        Serial.println(" mg ");

        // Print gyro values in degree/sec
        Serial.print("X-gyro rate: "); Serial.print(imu.gx, 3);
        Serial.print(" degrees/sec ");
        Serial.print("Y-gyro rate: "); Serial.print(imu.gy, 3);
        Serial.print(" degrees/sec ");
        Serial.print("Z-gyro rate: "); Serial.print(imu.gz, 3);
        Serial.println(" degrees/sec");

        // Print mag values in degree/sec
        Serial.print("X-mag field: "); Serial.print(imu.mx);
        Serial.print(" mG ");
        Serial.print("Y-mag field: "); Serial.print(imu.my);
        Serial.print(" mG ");
        Serial.print("Z-mag field: "); Serial.print(imu.mz);
        Serial.println(" mG");

        imu.tempCount = imu.readTempData();  // Read the adc values
        // Temperature in degrees Centigrade
        imu.temperature = ((float) imu.tempCount) / 333.87 + 21.0;
        // Print temperature in degrees Centigrade
        Serial.print("Temperature is ");  Serial.print(imu.temperature, 1);
        Serial.println(" degrees C");
      }

      imu.count = millis();
//      digitalWrite(myLed, !digitalRead(myLed));  // toggle led
    } // if (myIMU.delt_t > 500)
  } // if (!AHRS)
//  else
//  {
//    // Serial print and/or display at 0.5 s rate independent of data rates
//    myIMU.delt_t = millis() - myIMU.count;
//
//    // update LCD once per half-second independent of read rate
//    if (myIMU.delt_t > 500)
//    {
//      if(SerialDebug)
//      {
//        Serial.print("ax = "); Serial.print((int)1000*myIMU.ax);
//        Serial.print(" ay = "); Serial.print((int)1000*myIMU.ay);
//        Serial.print(" az = "); Serial.print((int)1000*myIMU.az);
//        Serial.println(" mg");
//
//        Serial.print("gx = "); Serial.print( myIMU.gx, 2);
//        Serial.print(" gy = "); Serial.print( myIMU.gy, 2);
//        Serial.print(" gz = "); Serial.print( myIMU.gz, 2);
//        Serial.println(" deg/s");
//
//        Serial.print("mx = "); Serial.print( (int)myIMU.mx );
//        Serial.print(" my = "); Serial.print( (int)myIMU.my );
//        Serial.print(" mz = "); Serial.print( (int)myIMU.mz );
//        Serial.println(" mG");
//
//        Serial.print("q0 = "); Serial.print(*getQ());
//        Serial.print(" qx = "); Serial.print(*(getQ() + 1));
//        Serial.print(" qy = "); Serial.print(*(getQ() + 2));
//        Serial.print(" qz = "); Serial.println(*(getQ() + 3));
//      }
//
//// Define output variables from updated quaternion---these are Tait-Bryan
//// angles, commonly used in aircraft orientation. In this coordinate system,
//// the positive z-axis is down toward Earth. Yaw is the angle between Sensor
//// x-axis and Earth magnetic North (or true North if corrected for local
//// declination, looking down on the sensor positive yaw is counterclockwise.
//// Pitch is angle between sensor x-axis and Earth ground plane, toward the
//// Earth is positive, up toward the sky is negative. Roll is angle between
//// sensor y-axis and Earth ground plane, y-axis up is positive roll. These
//// arise from the definition of the homogeneous rotation matrix constructed
//// from quaternions. Tait-Bryan angles as well as Euler angles are
//// non-commutative; that is, the get the correct orientation the rotations
//// must be applied in the correct order which for this configuration is yaw,
//// pitch, and then roll.
//// For more see
//// http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
//// which has additional links.
//      myIMU.yaw   = atan2(2.0f * (*(getQ()+1) * *(getQ()+2) + *getQ() *
//                    *(getQ()+3)), *getQ() * *getQ() + *(getQ()+1) * *(getQ()+1)
//                    - *(getQ()+2) * *(getQ()+2) - *(getQ()+3) * *(getQ()+3));
//      myIMU.pitch = -asin(2.0f * (*(getQ()+1) * *(getQ()+3) - *getQ() *
//                    *(getQ()+2)));
//      myIMU.roll  = atan2(2.0f * (*getQ() * *(getQ()+1) + *(getQ()+2) *
//                    *(getQ()+3)), *getQ() * *getQ() - *(getQ()+1) * *(getQ()+1)
//                    - *(getQ()+2) * *(getQ()+2) + *(getQ()+3) * *(getQ()+3));
//      myIMU.pitch *= RAD_TO_DEG;
//      myIMU.yaw   *= RAD_TO_DEG;
//      // Declination of SparkFun Electronics (40°05'26.6"N 105°11'05.9"W) is
//      //   8° 30' E  ± 0° 21' (or 8.5°) on 2016-07-19
//      // - http://www.ngdc.noaa.gov/geomag-web/#declination
//      myIMU.yaw   -= 8.5;
//      myIMU.roll  *= RAD_TO_DEG;
//
//      if(SerialDebug)
//      {
//        Serial.print("Yaw, Pitch, Roll: ");
//        Serial.print(myIMU.yaw, 2);
//        Serial.print(", ");
//        Serial.print(myIMU.pitch, 2);
//        Serial.print(", ");
//        Serial.println(myIMU.roll, 2);
//
//        Serial.print("rate = ");
//        Serial.print((float)myIMU.sumCount/myIMU.sum, 2);
//        Serial.println(" Hz");
//      }
//
//#ifdef LCD
//      display.clearDisplay();
//
//      display.setCursor(0, 0); display.print(" x   y   z  ");
//
//      display.setCursor(0,  8); display.print((int)(1000*myIMU.ax));
//      display.setCursor(24, 8); display.print((int)(1000*myIMU.ay));
//      display.setCursor(48, 8); display.print((int)(1000*myIMU.az));
//      display.setCursor(72, 8); display.print("mg");
//
//      display.setCursor(0,  16); display.print((int)(myIMU.gx));
//      display.setCursor(24, 16); display.print((int)(myIMU.gy));
//      display.setCursor(48, 16); display.print((int)(myIMU.gz));
//      display.setCursor(66, 16); display.print("o/s");
//
//      display.setCursor(0,  24); display.print((int)(myIMU.mx));
//      display.setCursor(24, 24); display.print((int)(myIMU.my));
//      display.setCursor(48, 24); display.print((int)(myIMU.mz));
//      display.setCursor(72, 24); display.print("mG");
//
//      display.setCursor(0,  32); display.print((int)(myIMU.yaw));
//      display.setCursor(24, 32); display.print((int)(myIMU.pitch));
//      display.setCursor(48, 32); display.print((int)(myIMU.roll));
//      display.setCursor(66, 32); display.print("ypr");
//
//    // With these settings the filter is updating at a ~145 Hz rate using the
//    // Madgwick scheme and >200 Hz using the Mahony scheme even though the
//    // display refreshes at only 2 Hz. The filter update rate is determined
//    // mostly by the mathematical steps in the respective algorithms, the
//    // processor speed (8 MHz for the 3.3V Pro Mini), and the magnetometer ODR:
//    // an ODR of 10 Hz for the magnetometer produce the above rates, maximum
//    // magnetometer ODR of 100 Hz produces filter update rates of 36 - 145 and
//    // ~38 Hz for the Madgwick and Mahony schemes, respectively. This is
//    // presumably because the magnetometer read takes longer than the gyro or
//    // accelerometer reads. This filter update rate should be fast enough to
//    // maintain accurate platform orientation for stabilization control of a
//    // fast-moving robot or quadcopter. Compare to the update rate of 200 Hz
//    // produced by the on-board Digital Motion Processor of Invensense's MPU6050
//    // 6 DoF and MPU9150 9DoF sensors. The 3.3 V 8 MHz Pro Mini is doing pretty
//    // well!
//      display.setCursor(0, 40); display.print("rt: ");
//      display.print((float) myIMU.sumCount / myIMU.sum, 2);
//      display.print(" Hz");
//      display.display();
//#endif // LCD
//
//      myIMU.count = millis();
//      myIMU.sumCount = 0;
//      myIMU.sum = 0;
//    } // if (myIMU.delt_t > 500)
//  } // if (AHRS)
}

