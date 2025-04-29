// Define pin connections
const int buzzerPin = 9;      // The pin connected to the buzzer
const int buttonDot = 2;      // The pin for the dot button
const int buttonDash = 3;     // The pin for the dash button
const int powerLed = 8;       // The pin for power led
const int potPin = A0;        // The pin connected to the potentiometer (analog input)

// Morse code timing - based on ARRL standards
int dotDuration = 100;        // Base duration of a dot in milliseconds
int dashDuration = dotDuration * 3;  // Dash is 3x dot duration
int elementGap = dotDuration; // Space between elements of same character
int charGap = dotDuration * 3; // Space between characters (minus element gap)
int wordGap = dotDuration * 7; // Space between words (minus char gap)

// Buzzer frequencies
const int DotHz = 800;        // Frequency for dot sound
const int DashHz = 800;       // Frequency for dash sound

// Timing variables
bool buzzerActive = false;    // Tracks if the buzzer is currently active
unsigned long buzzerStartTime = 0; // Stores start time for buzzer
int currentDuration = 0;      // Stores duration of the current sound
unsigned long lastSignalEnd = 0; // Time when last signal ended

// Text-to-Morse variables
String inputString = "";      // String to hold incoming serial data
bool stringComplete = false;  // Flag to indicate complete string received
bool textToMorseMode = false; // Flag for text-to-Morse mode

// Morse code lookup table (A-Z, 0-9)
const char* morseCodes[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",    // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",  // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",         // S-Z
  "-----", ".----", "..---", "...--", "....-", ".....", "-....",  // 0-6
  "--...", "---..", "----."                                        // 7-9
};

void setup() {
  pinMode(powerLed, OUTPUT);
  digitalWrite(powerLed, HIGH);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonDot, INPUT_PULLUP);
  pinMode(buttonDash, INPUT_PULLUP);
  Serial.begin(9600);
  
  // Print Morse code reference table
  printMorseTable();
  
  Serial.println("\nCW Keyer Ready");
  Serial.println("Send text to convert to Morse, or use buttons for manual keying");
  Serial.print("Current speed: ");
  Serial.print(1200 / dotDuration); // Calculate WPM
  Serial.println(" WPM");
}

void loop() {
  unsigned long currentTime = millis();

  // Read potentiometer and adjust timing
  int potValue = analogRead(potPin);
  dotDuration = map(potValue, 0, 1023, 50, 200); // 5-24 WPM range
  dashDuration = dotDuration * 3;
  elementGap = dotDuration;
  charGap = dotDuration * 3 - elementGap; // Total 3 units between chars
  wordGap = dotDuration * 7 - (charGap + elementGap * 3); // Total 7 units between words

  // Handle serial input for text-to-Morse
  if (stringComplete) {
    processTextToMorse(inputString);
    inputString = "";
    stringComplete = false;
  }

  // Manual keying mode (only if not in text-to-Morse mode)
  if (!textToMorseMode) {
    // Check if dot button is pressed and buzzer is not already active
    if (digitalRead(buttonDot) == LOW && !buzzerActive) {
      startDot(currentTime);
    }

    // Check if dash button is pressed and buzzer is not already active
    if (digitalRead(buttonDash) == LOW && !buzzerActive) {
      startDash(currentTime);
    }
  }

  // Turn off buzzer after the duration has elapsed
  if (buzzerActive && currentTime - buzzerStartTime >= currentDuration) {
    stopBuzzer();
    lastSignalEnd = currentTime;
  }
}

// Print Morse code reference table
void printMorseTable() {
  Serial.println("\nMorse Code Reference Table (Standard Timing)");
  Serial.println("===========================================");
  Serial.println("Dot length = 1 unit");
  Serial.println("Dash length = 3 units");
  Serial.println("Space between elements = 1 unit");
  Serial.println("Space between characters = 3 units");
  Serial.println("Space between words = 7 units");
  Serial.println("\nLetters:");
  
  // Print letters A-Z in columns
  for (int i = 0; i < 13; i++) {
    char c1 = 'A' + i;
    char c2 = (i < 13) ? 'N' + i : ' ';
    Serial.print(c1);
    Serial.print(": ");
    Serial.print(morseCodes[c1 - 'A']);
    Serial.print("\t");
    if (c2 <= 'Z') {
      Serial.print(c2);
      Serial.print(": ");
      Serial.print(morseCodes[c2 - 'A']);
    }
    Serial.println();
  }
  
  Serial.println("\nNumbers:");
  for (char c = '0'; c <= '9'; c++) {
    Serial.print(c);
    Serial.print(": ");
    Serial.println(morseCodes[c - '0' + 26]);
  }
}

// Function to process text string into Morse code with proper timing
void processTextToMorse(String text) {
  textToMorseMode = true;
  text.toUpperCase();
  
  for (unsigned int i = 0; i < text.length(); i++) {
    char c = text[i];
    
    if (c == ' ') {
      // Word space (7 units total)
      delay(wordGap);
      continue;
    }
    
    // Find Morse code for this character
    const char* morseChar = charToMorse(c);
    if (morseChar == NULL) continue; // Skip unknown characters
    
    // Play each element of the Morse character
    for (unsigned int j = 0; j < strlen(morseChar); j++) {
      // Wait appropriate gap between elements (except first one)
      if (j > 0) {
        delay(elementGap);
      }
      
      // Play dot or dash
      if (morseChar[j] == '.') {
        playDot();
      } else if (morseChar[j] == '-') {
        playDash();
      }
    }
    
    // Character space (3 units total, minus the last element gap)
    delay(charGap);
  }
  
  textToMorseMode = false;
}

// Play a dot with proper timing
void playDot() {
  tone(buzzerPin, DotHz);
  delay(dotDuration);
  noTone(buzzerPin);
}

// Play a dash with proper timing
void playDash() {
  tone(buzzerPin, DashHz);
  delay(dashDuration);
  noTone(buzzerPin);
}

// Convert character to Morse code
const char* charToMorse(char c) {
  if (c >= 'A' && c <= 'Z') {
    return morseCodes[c - 'A'];
  } else if (c >= '0' && c <= '9') {
    return morseCodes[c - '0' + 26];
  }
  return NULL; // Unknown character
}

// Function to start a dot signal
void startDot(unsigned long currentTime) {
  buzzerActive = true;
  buzzerStartTime = currentTime;
  currentDuration = dotDuration;
  tone(buzzerPin, DotHz);
}

// Function to start a dash signal
void startDash(unsigned long currentTime) {
  buzzerActive = true;
  buzzerStartTime = currentTime;
  currentDuration = dashDuration;
  tone(buzzerPin, DashHz);
}

// Function to stop the buzzer after signal duration ends
void stopBuzzer() {
  noTone(buzzerPin);
  buzzerActive = false;
}

// SerialEvent occurs whenever new data comes in the hardware serial RX
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}