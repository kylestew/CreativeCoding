/* HOOKUP GUIDE
  (be aware the wire colors on the PIR sensor are wrong)
  pull up the alarm pin to 5+V
  D2 --- AL
*/
#include <MicroView.h>

int AL = 2; // D2

void setup() {
  pinMode(AL, INPUT);
  
  uView.begin();
  uView.clear(PAGE);
}

void loop() {
  int dat = digitalRead(AL) ? 0 : 0xFF;
  
  for (int i = 0; i < 6; i++) {
    uView.setPageAddress(i);
    uView.setColumnAddress(0);
    for (int j = 0; j < 0x40; j++) {
      uView.data(dat);
    }
  }
  
  delay(100);
}
