int buttonPin = 9;
int buttonState = 0;
int mode = 0;

int irPin1 = A1; //analog inputs
int irPin2 = A2;
int irPin3 = A3;
int irPin4 = A4;
int irPin5 = A5;
int irPin6 = A6;
int irPin7 = A7;
int irPin8 = A8;

int dist1 = 0; //distance readings
int dist2 = 0;
int dist3 = 0;
int dist4 = 0;
int dist5 = 0;
int dist6 = 0;
int dist7 = 0;
int dist8 = 0;

int base1 = 0; //distances from wall
int base2 = 0;
int base3 = 0;
int base4 = 0;
int base5 = 0;
int base6 = 0;
int base7 = 0;
int base8 = 0;

const int solenoidTPin1 = 4; //transistors for solenoids
const int solenoidTPin2 = 5;
const int solenoidTPin3 = 6;
const int solenoidTPin4 = 7;
const int solenoidTPin5 = 8;
const int solenoidTPin6 = 9;
const int solenoidTPin7 = 10;
const int solenoidTPin8 = 11;

int penta[] = {1, 2, 3, 5, 6, 8, 6, 5, 3, 2, 1}; //tutorial mode song
int i = 0; //index of penta[]; determines which note to play

const int ledPin1 = 31; //LED lights
const int ledPin2 = 32;
const int ledPin3 = 33;
const int ledPin4 = 34;
const int ledPin5 = 35;
const int ledPin6 = 36;
const int ledPin7 = 37;
const int ledPin8 = 38;

//int howManyKeys = 0; // number of keys that are playing

//--------------------------------------------------
void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(solenoidTPin1, OUTPUT);
  pinMode(solenoidTPin2, OUTPUT);
  pinMode(solenoidTPin3, OUTPUT);
  pinMode(solenoidTPin4, OUTPUT);
  pinMode(solenoidTPin5, OUTPUT);
  pinMode(solenoidTPin6, OUTPUT);
  pinMode(solenoidTPin7, OUTPUT);
  pinMode(solenoidTPin8, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);
  pinMode(ledPin6, OUTPUT);
  pinMode(ledPin7, OUTPUT);
  pinMode(ledPin8, OUTPUT);
}
//--------------------------------------------------
// plays a note and flashes the accompanying light
void strikeNote(int note, int light) {
  digitalWrite(light, HIGH); //light on
  digitalWrite(note, LOW); //solenoid on (play note)
  delay(200);
  digitalWrite(note, HIGH); //solenoid off
  delay(300);
  digitalWrite(light, HIGH); //light off
  delay(500);
}
//--------------------------------------------------
// flash all lights; signals calibration or end of tutorial mode
void blink8 () {
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);
  digitalWrite(ledPin4, HIGH);
  digitalWrite(ledPin5, HIGH);
  digitalWrite(ledPin6, HIGH);
  digitalWrite(ledPin7, HIGH);
  digitalWrite(ledPin8, HIGH);
  delay(1000);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);
  digitalWrite(ledPin5, LOW);
  digitalWrite(ledPin6, LOW);
  digitalWrite(ledPin7, LOW);
  digitalWrite(ledPin8, LOW);
}
//--------------------------------------------------
void loop() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) { mode++; } //pushing the button increments mode
  if (mode > 2) { mode = 0; } //if mode > 2, resets to default mode
  
  // (constantly) read distances
  dist1 = analogRead(irPin1);
  dist2 = analogRead(irPin2);
  dist3 = analogRead(irPin3);
  dist4 = analogRead(irPin4);
  dist5 = analogRead(irPin5);
  dist6 = analogRead(irPin6);
  dist7 = analogRead(irPin7);
  dist8 = analogRead(irPin8);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~
  // calibration mode
  if (mode == 1) {
    Serial.println("SETTING BASES");
    base1 = dist1;
    base2 = dist2;
    base3 = dist3;
    base4 = dist4;
    base5 = dist5;
    base6 = dist6;
    base7 = dist7;
    base8 = dist8;
    blink8(); // flash lights
    delay(1000);
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~
    // tutorial mode
  } else if (mode == 2) {
    delay(100);
    
    // switch statement determines which note (index of penta[]) to play
    switch (penta[i]) {
      case 1: // F1
        digitalWrite(ledPin1, HIGH);
        delay(100);
        if (dist1 > (base1+80)) { //someone stepped on stair F1

          i++; //increment i to play next note in song
        }
        break;
      case 2: // G
        digitalWrite(ledPin2, HIGH);
        delay(100);
        if (dist2 > (base2+80)) { //someone stepped on stair G
          strikeNote(solenoidTPin2, ledPin2); //play note (automatically turns off LED)
          i++; //increment i to play next note in song
        }
        break;
      case 3: // A
        digitalWrite(ledPin3, HIGH); //light up LED
        delay(100);
        if (dist3 > (base3+80)) { //someone stepped on stair A
          strikeNote(solenoidTPin3, ledPin3); //play note (automatically turns off LED)
          i++; //increment i to play next note in song
        }
        break;
      case 4: // A#
        digitalWrite(ledPin4, HIGH); //light up LED
        delay(100);
        if (dist4 > (base4+80)) { //someone stepped on stair A#
          strikeNote(solenoidTPin4, ledPin4); //play note (automatically turns off LED)
          i++; //increment i to play next note in song
        }
        break;
      case 5: // C
        digitalWrite(ledPin5, HIGH); //light up LED
        delay(100);
        if (dist5 > (base5+80)) { //someone stepped on stair C
          strikeNote(solenoidTPin5, ledPin5); //play note (automatically turns off LED)
          i++; //increment i to play next note in song
        }
        break;
      case 6: // D
        digitalWrite(ledPin6, HIGH); //light up LED
        delay(100);
        if (dist6 > (base6+80)) { //someone stepped on stair D
          strikeNote(solenoidTPin6, ledPin6); //play note (automatically turns off LED)
          i++; //increment i to play next note in song
        }
        break;
      case 7: // E
        digitalWrite(ledPin7, HIGH); //light up LED
        delay(100);
        if (dist7 > (base7+80)) { //someone stepped on stair E
          strikeNote(solenoidTPin7, ledPin7); //play note (automatically turns off LED)
          i++; //increment i to play next note in song
        }
        break;
      case 8: // F2
        digitalWrite(ledPin8, HIGH); //light up LED
        delay(100);
        if (dist8 > (base8+80)) { //someone stepped on stair F2
          strikeNote(solenoidTPin8, ledPin8); //play note (automatically turns off LED)
          i++; //increment i to play next note in song
        }
        break;
    } // close switch statement
    
    // at end of tutorial mode
    if (i == 11) {
      i = 0; //reset i to 0
      mode = 0; //switch do default mode
      blink8(); // flash lights
    }
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~
    // default mode
  } else {
    if (dist1 > (base1+80)) { //F1
      strikeNote(solenoidTPin1, ledPin1);
    } else if (dist2 > (base2+80)) { //G
      strikeNote(solenoidTPin2, ledPin2);
    } else if (dist3 > (base3+80)) { //A
      strikeNote(solenoidTPin3, ledPin3);      
    } else if (dist4 > (base4+80)) { //A#
      strikeNote(solenoidTPin4, ledPin4);
    } else if (dist5 > (base5+80)) { //C
      strikeNote(solenoidTPin5, ledPin5);
    } else if (dist6 > (base6+80)) { //D
      strikeNote(solenoidTPin6, ledPin6);
    } else if (dist7 > (base7+80)) { //E
      strikeNote(solenoidTPin7, ledPin7);
    } else if (dist8 > (base8+80)) { //F2
      strikeNote(solenoidTPin8, ledPin8);
    }
  } // end else (default mode)
} // end loop()


