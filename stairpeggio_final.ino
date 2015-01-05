// PHYSICAL SYSTEM CONSTANTS
int numKeys = 8;
int irPin[] = {A1,A3,A5,A7,A9,A11,A13,A15};
int solenoidPin[] = {22,26,30,34,38,42,46,50};
int ledPin[] = {25,29,33,37,41,45,49,53};
int threshold = 120;
int solenoidDuration = 24;
int ledDuration = 300;
unsigned long systemStart;
unsigned long lastSystemPrint = 0;

// MODE VARIABLES
int mode = -1; //Mode 0 is calibration, 1 is normal operation, 2 is demo mode
int stairpeggioIndex = 0;
int demoIndex = 0;
const int lengthOfDemo = 11;
int penta[] = {7, 6, 5, 3, 2, 0, 2, 3, 5, 6, 7};

// BUTTON VARIABLES
int buttonPin = 9;
int lastButtonReading = 0;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 500;    // the debounce time; increase if the output flickers

// INPUT VARIABLES
int index = 0;
int readings_row_length = 50;
int readings[8][50];
int dist[] = {0,0,0,0,0,0,0,0};
int base[] = {0,0,0,0,0,0,0,0};
int goodCalibration = 0;

// STATE VARIABLES
boolean irState[] = {0,0,0,0,0,0,0,0};
boolean irPreviousState[] = {0,0,0,0,0,0,0,0};
boolean ledState[] = {0,0,0,0,0,0,0,0};
boolean solenoidState[] = {0,0,0,0,0,0,0,0};
unsigned long solenoidHistory[] = {0,0,0,0,0,0,0,0};
unsigned long ledHistory[] = {0,0,0,0,0,0,0,0};
unsigned long ledTimer[] = {0,0,0,0,0,0,0,0};

// ------------------------------------------------------------------//
// LED HELPER FUNCTIONS
// ------------------------------------------------------------------//

void ledOn(int key, unsigned long time = 300){
  digitalWrite(ledPin[key], HIGH);
  ledState[key] = 1;
  ledHistory[key] = millis();
  ledTimer[key] = time;
}
void ledOff(int key){
  digitalWrite(ledPin[key], LOW);
  ledState[key] = 0;
  ledHistory[key] = millis();
}

void ledAllOn(){
  for (int i = 0; i < numKeys; i++){
    ledOn(i);
  }
}
void ledAllOff(){
  for (int i = 0; i < numKeys; i++){
    ledOff(i);
  }
}

// ------------------------------------------------------------------//
// SOLENOID HELPER FUNCTIONS
// ------------------------------------------------------------------//

void solenoidOn(int key){
  digitalWrite(solenoidPin[key], LOW);
  solenoidState[key] = 1;
  solenoidHistory[key] = millis();
}
void solenoidOff(int key){
  digitalWrite(solenoidPin[key], HIGH);
  solenoidState[key] = 0;
  solenoidHistory[key] = millis();
}
void solenoidAllOff(){
  for (int i = 0; i < numKeys; i++){
    solenoidOff(i);  
  }
}

// ------------------------------------------------------------------//
//INPUT HELPER FUNCTIONS
// ------------------------------------------------------------------//

void clearKeyReadings(int key){
  for (int i = 0; i < readings_row_length; i++){
    readings[key][i] = i;
  }
}
void clearAllKeyReadings(){
  for (int i = 0; i < numKeys; i++){
    clearKeyReadings(i);
  }
}
void takeReadings(){
  for (int key = 0; key < numKeys; key++){
    readings[key][index] = analogRead(irPin[key]);
  }
  index++;
  if (index > 50){index = 0;}
}
void computeDistances(){
  int sum[] = {0,0,0,0,0,0,0,0};
  for (int key = 0; key < numKeys; key++ ){
    for (int i = 0; i < readings_row_length; i++ ){
      sum[key] += readings[key][i];
    }
    dist[key] = sum[key] / readings_row_length;
  }
}
void setBases(){
  int sum = 0;
  for (int key = 0; key < numKeys; key++ ){
    sum += dist[key];
  }
  int ave = sum / numKeys;
  for (int key = 0; key < numKeys; key++ ){
    if ((abs(dist[key] - ave)) > 25){
      return;
    }
  }
  for (int key = 0; key < numKeys; key++ ){
    base[key] = dist[key];
  }
  ledOn(2,100);
  ledOn(6,100);
  goodCalibration++;
}

// ------------------------------------------------------------------//
// REACTION HELPER FUNCTIONS
// ------------------------------------------------------------------//

void reactToIR(){
  for (int key = 0; key < numKeys; key++ ){
    reactToIRKey(key);
  }
}
boolean reactToIRKey(int key){
  boolean triggered = 0;
  if (dist[key] > (base[key]+threshold)){
    irState[key] = 1;
  }else if (dist[key] < (base[key]+(threshold/8))){
    irState[key] = 0;
  }
  if ((irState[key] > irPreviousState[key]) && ((millis() - solenoidHistory[key]) > 250)){
    solenoidOn(key);
    ledOn(key);
    printSystemState();
    triggered = 1;
  }
  irPreviousState[key] = irState[key];
  return triggered;
}
void reactToState(){
  for (int key = 0; key < numKeys; key++ ){
    if ((solenoidState[key] == 1) && ((millis() - solenoidHistory[key]) > solenoidDuration)){
      solenoidOff(key);
    }
    if ((ledState[key] == 1) && ((millis() - ledHistory[key]) > ledTimer[key])){
      ledOff(key);
    }
  }
}

// ------------------------------------------------------------------//
// MODE HELPER FUNCTIONS
// ------------------------------------------------------------------//
void updateMode(){
  int reading = digitalRead(buttonPin);
  if (reading > lastButtonReading){
    if ((millis() - lastDebounceTime) > debounceDelay) {
      mode++;
      goodCalibration = 0;
      lastDebounceTime = millis(); 
    }
  }
  lastButtonReading = reading;
  if (mode > 2){mode = 0;}
}
// ------------------------------------------------------------------//
// PRINT HELPER FUNCTIONS
// ------------------------------------------------------------------//

void printSystemState(){
    String ss = "Solenoid states: (";
    for (int key = 0; key < (numKeys-1); key++){
      ss += String(solenoidState[key]) + ",";
    }
    ss += String(solenoidState[(numKeys-1)]) + ")";
    Serial.println(ss);
    
    String ds = "Dist: (";
    for (int key = 0; key < (numKeys-1); key++){
      ds += String(dist[key]) + ",";
    }
    ds += String(dist[(numKeys-1)]) + ")";
    Serial.println(ds);
    
    String bs = "Base: (";
    for (int key = 0; key < (numKeys-1); key++){
      bs += String(base[key]) + ",";
    }
    bs += String(base[(numKeys-1)]) + ")";
    Serial.println(bs);
    
    lastSystemPrint = millis();
}

// ------------------------------------------------------------------//
// MAIN FUNCTIONS
// ------------------------------------------------------------------//
void setup(){
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  for (int i = 0; i < numKeys; i++){
    pinMode(ledPin[i], OUTPUT);
    pinMode(solenoidPin[i], OUTPUT);
  }
  solenoidAllOff();
  ledAllOff();
  clearAllKeyReadings();
  
  systemStart = millis();
}

void loop(){
  updateMode();
  takeReadings();
  computeDistances();
  switch (mode){
    case -1:
      if ((millis() > ((unsigned long)(4000 + 200*stairpeggioIndex) + systemStart))){
        int key;
        if (stairpeggioIndex < numKeys){key = (numKeys-1) - stairpeggioIndex;}
        else {key = stairpeggioIndex - numKeys;}
        if (key >= 8){break;}
        ledOn(key);
        solenoidOn(key);
        stairpeggioIndex++;
      }
      if (stairpeggioIndex == (2*numKeys)){
        mode++;
      }
      reactToState();
      break;
    case 0:
      Serial.println(goodCalibration);
      setBases();
      reactToState();
        if (goodCalibration > 100){
          ledOn(1);
          ledOn(5);
        }
      break;
    case 1:
      reactToIR();
      reactToState();
      break;
    case 2:
      boolean triggered = 0;
      ledOn(penta[demoIndex]);
      triggered  = reactToIRKey(penta[demoIndex]);
      reactToState();
      if (triggered){demoIndex++;}
      if (demoIndex == (lengthOfDemo)){
        ledAllOn();
        mode = 1;
      }
      break;
  }  // close switch statement
  delay(2);
}
