// --- Pin Definitions ---
// Segment Pins (Shared across both displays)
const int segA = 2;
const int segB = 3;
const int segC = 4;
const int segD = 5;
const int segE = 6;
const int segF = 7;
const int segG = 8;
const int segDP = 9;

const int segmentPins[8] = {segA, segB, segC, segD, segE, segF, segG, segDP};

// Digit Control Pins (Transistor Bases)
const int leftDigit = 10;
const int rightDigit = 11;

// Button Pin
const int buttonPin = 12;

// --- Truth Table for Common Cathode (HIGH = ON) ---
const byte numbers[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

// --- Timer Variables ---
unsigned long previousMillis = 0;
int prevSeconds = 0;
int prevMin = 0;
const long interval = 1000; // 1 second update interval
int currentSec = 0;        // Timer counter (0-99)
bool isRunning = false;     // Timer state
int currentMin = 0;
int currentHour = 0;
bool hoursView = false;

// --- Button Variables ---
bool buttonState = HIGH;     
bool lastButtonState = HIGH;  
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;
bool pressedButton = false;
bool releasedButton = false;

void setup() {
  // Initialize segment pins as outputs
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  
  // Initialize digit control pins as outputs
  pinMode(leftDigit, OUTPUT);
  pinMode(rightDigit, OUTPUT);
  
  // Initialize button pin with internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

unsigned long now = 0;
int tens = 0;
int ones = 0;

void loop() {
  now = millis();
  handleButton(now);
  updateTimer();
  
  if(hoursView){
    tens = currentHour / 10;
    ones = currentHour % 10;
    //Serial.println("Min view");
}
else{
  tens = currentMin / 10;
  ones = currentMin % 10;
  //Serial.println("Secc view");
}
  
  //Serial.print(ones);
  //Serial.println(tens);
  
  displayDigit(leftDigit, tens);
  delay(5); 
  displayDigit(rightDigit, ones);
  delay(5);
}

// --- Helper Functions ---

void displayDigit(int digitPin, int number) {
  // 1. Turn OFF both digits to prevent ghosting
  digitalWrite(leftDigit, LOW);
  digitalWrite(rightDigit, LOW);
  
  // 2. Set the segments for the desired number
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], numbers[number][i]);
  }
  
  
  if (!isRunning){
    digitalWrite(segDP, LOW);
  }
  else{
  if(hoursView){
  
    if(digitPin == leftDigit){
      digitalWrite(segDP, HIGH);
    }
    else{
      digitalWrite(segDP, LOW);
    }
  }
  else{
    if(digitPin == rightDigit){
      digitalWrite(segDP, HIGH);
    }
    else{
      digitalWrite(segDP, LOW);
    }
  }
  }
  
  // 3. Turn ON the selected digit (HIGH activates NPN transistor)
  digitalWrite(digitPin, HIGH);
}

void updateTimer() 
{
  if (isRunning) 
  {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        currentSec++;
      }

    if (currentSec >= 60)
    {
      // prevSeconds = currentSec;
      currentMin++;
      currentSec = currentSec -60;
    }

    if (currentMin >= 60)
    {
      // prevMin = currentMin;
      currentHour++;
      currentMin = currentMin -60;
    }


  }
}

unsigned long changedAt = 0;
unsigned long downAt = 0;

void updateButton(unsigned long now) {
  pressedButton = false;
  releasedButton = false;
  bool reading = digitalRead(buttonPin);
  
  if (reading != buttonState) {
    changedAt = now;
    buttonState = reading;
  }
  
  if ((now - changedAt) > debounceDelay) {

    if (reading != lastButtonState) {
      lastButtonState = buttonState;
      
      if (lastButtonState == LOW) {
        pressedButton = true;
        downAt = now;
      }
      else{
        releasedButton = true;
      }
    }
  }
}

bool resetHandled = false;
int RESET_HOLD_MS = 5000;
int VIEW_HOLD_MS = 2000;


void resetTimer() {
  currentSec = 0;
  currentMin = 0;
  currentHour = 0;
  previousMillis = 0;
  prevMin = 0;

  isRunning = false;
  
  hoursView = false;
}

void handleButton(unsigned long now){
  updateButton(now);
  if(pressedButton){
    resetHandled = false;
    isRunning = !isRunning;
  }
  if (lastButtonState == LOW && !resetHandled &&
      static_cast<unsigned long>(now - downAt) >= RESET_HOLD_MS) {
     resetTimer(); 
     resetHandled = true;
  }
  if (releasedButton && !resetHandled) {
  unsigned long held = now - downAt;
  if (held >= VIEW_HOLD_MS) hoursView = !hoursView;
  //  else if (secondsElapsed < MAX_SECONDS) isRunning = !isRunning;
  }


}