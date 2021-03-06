// Binary Reader
// v.1: John Harrison 20Feb2015

// Pins 2-6 correspond to the binary i.e
// Example: Pin 2 which is 1s digit will be low for 1 and high for 0
// Start the arduino with no card in the reader

// TWEAK HERE:
#define MIN_TIME_BEFORE_ANOTHER_LETTER 25 // ms

// STATES:
#define NO_CARD 0
#define BETWEEN_LETTERS 1
#define READING_LETTER 2

// MSGS
#define START_CARD_TOKEN 30
#define NO_CARD_TOKEN 31

void setup() {
  Serial.begin(9600);
  // turn on pullup resistors in input pins
  for (uint8_t i=2;i<7;i++) {
    digitalWrite(i, HIGH);
  }
}

void loop() {
  static uint8_t lastState = NO_CARD;
  static uint8_t lastNumber = 31;
  static bool powersOf2[] = {1,1,1,1,1};

  uint8_t number = getNumber(powersOf2);
  uint8_t state = getState(number);
  if (state != lastState) {
    switch (state) {
    case NO_CARD:
      send(NO_CARD_TOKEN);
      break;
    case BETWEEN_LETTERS:
      if (lastState == NO_CARD) {
	send(START_CARD_TOKEN);
	delay(MIN_TIME_BEFORE_ANOTHER_LETTER);
      } else {
	send(lastNumber);
	delay(MIN_TIME_BEFORE_ANOTHER_LETTER);
      }
      zeroPowersOf2(powersOf2);
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
   bool noDots = true;
   for (uint8_t i=0;i<5;i++) {
     uint8_t pinState = digitalRead(i+2);
     for (uint8_t j = 0; j < 31; j++) {
       pinState &= digitalRead(i+2);
     }
     powersOf2[i] |= !pinState;
     noDots &= pinState;
     number += (1<<i)*powersOf2[i];
   }
   number = (noDots ? 0 : number);
   return number;
 }
 
void zeroPowersOf2(bool *powersOf2) {
   for (int i=0; i<5;i++) {
     powersOf2[i] = 0;
   }
 }

 void send(uint8_t number) {
   Serial.write(number+'A'-1);
 }
