#include <MicroView.h>

const int VERT = 0; // analog
const int HORIZ = 1; // analog
const int SEL = 2; // digital

MicroViewWidget* vertSlider;
MicroViewWidget* horizSlider;
MicroViewWidget* selSlider;

void setup() {
  pinMode(SEL, INPUT);
  digitalWrite(SEL, HIGH);

  uView.begin();
  uView.clear(PAGE);
  
  vertSlider = new MicroViewSlider(42, 0, 0, 1023, WIDGETSTYLE3 + WIDGETNOVALUE);
  horizSlider = new MicroViewSlider(0, 12, 0, 1023, WIDGETNOVALUE);
  selSlider = new MicroViewSlider(0, 32, 0, , WIDGETNOVALUE);
}

void loop() {
  int vertical, horizontal, select;
  
  vertical = analogRead(VERT); // will be 0-1023
  horizontal = analogRead(HORIZ); // will be 0-1023
  select = digitalRead(SEL); // will be HIGH (1) if not pressed, and LOW (0) if pressed

  vertSlider->setValue(vertical);
  horizSlider->setValue(1024-horizontal);
  selSlider->setValue(select);
    
  uView.display();
}  
