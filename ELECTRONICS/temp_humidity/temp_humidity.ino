/* HOOKUP GUIDE
  SDA ----- A4
  SCL ----- A5
  +   ----- 3V  not provided on microview :(
*/
#include <Wire.h>
#include <MicroView.h>
#include <SparkFunHTU21D.h>

HTU21D humidity;

void setup() {
  humidity.begin();
}

void loop() {
  float humd = humidity.readHumidity();
  float temp = humidity.readTemperature();
  temp = temp * (9/5.0) + 32;
  
  uView.begin();
  uView.clear(PAGE);
  uView.print("humidity:\n");
  uView.print(humd, 1);
  uView.print("%\n");

  uView.print('\n');
  uView.print("temp:\n");
  uView.print(temp, 1);
  uView.print('F');
  
  uView.display();

  delay(1000);
}
