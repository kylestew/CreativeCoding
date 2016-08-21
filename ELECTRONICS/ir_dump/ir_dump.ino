#define IR_PIN      PIND  // PORTD input registers
#define IR          2 // actual pin used on PORTD

#define RESOLUTION   10
#define MAXPULSE     65000

uint16_t pulses[255];
uint8_t currentpulse = 0;

void setup() {
//  pinMode(IRpin, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  uint16_t highpulse, lowpulse;
  highpulse = lowpulse = 0;
  
  // user PORT REGISTERS for fast access
  while (IR_PIN & _BV(IR)) { // HIGH
    highpulse++;
    delayMicroseconds(RESOLUTION);
    
    // timeout - won't move past this loop until we get a low signal
    if ((highpulse >= MAXPULSE) && (currentpulse != 0)) {
      printpulses();
      currentpulse = 0;
      return;
    }
  }
  pulses[currentpulse] = highpulse;
  
  while (!(IR_PIN & _BV(IR))) { // LOW
    lowpulse++;
    delayMicroseconds(RESOLUTION);
    
    if ((lowpulse >= MAXPULSE) && (currentpulse != 0)) {
      printpulses();
      currentpulse = 0;
      return;
    }
  }
  
  currentpulse++;
}

void printpulses(void) {
//  Serial.println("\n\r\n\rReceived: \n\rOFF \tON");
  for (uint8_t i = 0; i < currentpulse; i++) {
    uint16_t usecs = pulses[i] * RESOLUTION;
    // 1100 = 1
    // 320 = 0
    // midpoint = ~700
    int binval = usecs < 700 ? 0 : 1;
   if (usecs > 1250) {
     binval = 2;
   } 
    Serial.print(binval, DEC);
  }
  Serial.println("");
  Serial.print(currentpulse, DEC);
  Serial.println(" bits");
  
//  // print it in a 'array' format
//  Serial.println("int IRsignal[] = {");
//  Serial.println("// ON, OFF (in 10's of microseconds)");
//  for (uint8_t i = 0; i < currentpulse-1; i++) {
//    Serial.print("\t"); // tab
//    Serial.print(pulses[i][1] * RESOLUTION / 10, DEC);
//    Serial.print(", ");
//    Serial.print(pulses[i+1][0] * RESOLUTION / 10, DEC);
//    Serial.println(",");
//  }
//  Serial.print("\t"); // tab
//  Serial.print(pulses[currentpulse-1][1] * RESOLUTION / 10, DEC);
//  Serial.print(", 0};");
}
