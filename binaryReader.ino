// Pins 2-6 correspond to the binary i.e
// Pin 2 which is 1s digit will be low when on

// STATES:
#define NO_CARD 0
#define BETWEEN_LETTERS 1
#define READING_LETTER 2

void setup() {
  Serial.begin(115200);
  // turn on pullup resistors in input pins
  for (uint8_t i=2;i<7;i++) {
    digitalWrite(i, HIGH);
  }
}

void loop() {
  static bool powersOf2[] = {0,0,0,0,0};
  static uint8_t lastState = NO_CARD;
  static uint8_t lastNumber = 31;

  uint8_t number = getNumber(powersOf2);
  uint8_t state = getState(number);
  if (state != lastState) {
    switch (state) {
    case NO_CARD:
      zeroPowersOf2(powersOf2);
      send(number);
      break;
    case BETWEEN_LETTERS:
      send(lastNumber);
      break;
    }
  }
  lastNumber = number;
  lastState = state;
}
 
 uint8_t getState(uint8_t number) {
   switch (number) {
   case 0:
     return BETWEEN_LETTERS;
   case 31:
     return NO_CARD;
   default:
     return READING_LETTER;
   }
 }
 
 uint8_t getNumber(bool *powersOf2) {
   uint8_t number = 0;
   for (uint8_t i=0;i<5;i++) {
     powersOf2[i] &= !digitalRead(i+2);
     number = (1<<i)*powersOf2[i];
   }
   return number;
 }
 
 uint8_t zeroPowersOf2(bool *powersOf2) {
   for (int i=0; i<5;i++) {
     powersOf2[i] = 0;
   }
 }

 void send(uint8_t number) {
   Serial.write(number+'A'-1);
 }
