/* HOOKUP GUIDE
 +5V - +5V
 GND - GND
 AN - Analog In 0
*/
#include <MicroView.h>

MicroViewWidget *guage;

void setup() {
  uView.begin();		
  uView.clear(PAGE);
  
  // max read on sensor is 254"
  guage = new MicroViewGauge(32,24,0,50, WIDGETSTYLE0);
}

void loop() {
  int sensor, inches, x;
  
  sensor = analogRead(0);
  inches = sensor / 2;
  
  guage->setValue(inches);
  uView.display();
  
  delay(100);                     
}

