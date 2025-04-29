// Define pin connections
const int buzzerPin = 9;      // The pin connected to the buzzer
const int buttonDot = 2;      // The pin for the dot button
const int buttonDash = 3;     // The pin for the dash button

// Define Morse code timing
const int dotDuration = 200;  // Duration of a dot in milliseconds
const int dashDuration = 600; // Duration of a dash in milliseconds

// Variables for timing control
bool buzzerActive = false;    // Tracks whether the buzzer is currently active
unsigned long startTime = 0;  // Stores the starting time for the buzzer
int currentDuration = 0;      // Stores the duration of the current sound

void setup() {
    // Set pin modes
    pinMode(buzzerPin, OUTPUT);        // Buzzer as output
    pinMode(buttonDot, INPUT_PULLUP);  // Dot button as input with pull-up resistor
    pinMode(buttonDash, INPUT_PULLUP); // Dash button as input with pull-up resistor
}

void loop() {
    // Get the current time in milliseconds
    unsigned long currentTime = millis();

    // If dot button is pressed and the buzzer is not active, start a dot signal
    if (digitalRead(buttonDot) == LOW && !buzzerActive) {
        buzzerActive = true;            // Mark the buzzer as active
        startTime = currentTime;        // Store the time when the signal started
        currentDuration = dotDuration;  // Set duration to dot time
        tone(buzzerPin, 1000);          // Activate buzzer with 1000 Hz frequency
    }

    // If dash button is pressed and the buzzer is not active, start a dash signal
    if (digitalRead(buttonDash) == LOW && !buzzerActive) {
        buzzerActive = true;            // Mark the buzzer as active
        startTime = currentTime;        // Store the time when the signal started
        currentDuration = dashDuration; // Set duration to dash time
        tone(buzzerPin, 1000);          // Activate buzzer with 1000 Hz frequency
    }

    // If buzzer is active and the time has passed, turn it off
    if (buzzerActive && currentTime - startTime >= currentDuration) {
        noTone(buzzerPin);  // Stop buzzer
        buzzerActive = false;  // Reset buzzer state
    }
}