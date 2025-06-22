// Define pin connections
const int buzzerPin = 9;      // The pin connected to the buzzer
const int buttonDot = 2;      // The pin for the dot button
const int buttonDash = 3;     // The pin for the dash button

// Define Morse code timing
const int dotDuration = 200;  // Duration of a dot in milliseconds
const int dashDuration = 600; // Duration of a dash in milliseconds
const int gapDuration = 150;  // Short gap between signals

// Define buzzer frequencies
const int DotHz = 800;  // Frequency for dot sound
const int DashHz = 800; // Frequency for dash sound

// Timing variables
bool buzzerActive = false;       // Tracks if the buzzer is currently active
unsigned long buzzerStartTime = 0; // Stores start time for buzzer
int currentDuration = 0;         // Stores duration of the current sound

void setup() {
    pinMode(buzzerPin, OUTPUT);
    pinMode(buttonDot, INPUT_PULLUP);
    pinMode(buttonDash, INPUT_PULLUP);
}

void loop() {
    unsigned long currentTime = millis(); // Get the current time

    // Check if dot button is pressed and buzzer is not already active
    if (digitalRead(buttonDot) == LOW && !buzzerActive) {
        startDot(currentTime);
    }

    // Check if dash button is pressed and buzzer is not already active
    if (digitalRead(buttonDash) == LOW && !buzzerActive) {
        startDash(currentTime);
    }

    // Turn off buzzer after the duration has elapsed
    if (buzzerActive && currentTime - buzzerStartTime >= currentDuration) {
        stopBuzzer();
    }
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