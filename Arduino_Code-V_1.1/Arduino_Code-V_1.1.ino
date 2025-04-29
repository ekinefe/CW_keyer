// Define pin connections
const int buzzerPin = 9;      // The pin connected to the buzzer
const int buttonDot = 2;      // The pin for the dot button
const int buttonDash = 3;     // The pin for the dash button

// Define Morse code timing
const int dotDuration = 200;  // Duration of a dot in milliseconds
const int dashDuration = 600; // Duration of a dash in milliseconds
const int gapDuration = 150;  // Short gap between signals for readability

// Variables for tracking buzzer state
bool buzzerActive = false;    // Flag to check if the buzzer is active
unsigned long startTime = 0;  // Timestamp when the buzzer starts
int currentDuration = 0;      // Stores the duration of the current Morse signal
int DotHz = 800;
int DashHz = 800;

void setup() {
    // Set pin modes for input and output
    pinMode(buzzerPin, OUTPUT);        // Set buzzer pin as an output
    pinMode(buttonDot, INPUT_PULLUP);  // Configure dot button as input with pull-up resistor
    pinMode(buttonDash, INPUT_PULLUP); // Configure dash button as input with pull-up resistor
}

void loop() {
    // Get the current time in milliseconds for non-blocking execution
    unsigned long currentTime = millis();

    // Check if the dot button is pressed and buzzer is not already active
    if (digitalRead(buttonDot) == LOW && !buzzerActive) {
        buzzerActive = true;            // Activate buzzer state tracking
        startTime = currentTime;        // Store the start time of the signal
        currentDuration = dotDuration;  // Set signal duration to dot timing
        tone(buzzerPin, DotHz);           // Play tone at 800 Hz for dot
    }

    // Check if the dash button is pressed and buzzer is not already active
    if (digitalRead(buttonDash) == LOW && !buzzerActive) {
        buzzerActive = true;            // Activate buzzer state tracking
        startTime = currentTime;        // Store the start time of the signal
        currentDuration = dashDuration; // Set signal duration to dash timing
        tone(buzzerPin, DashHz);           // Play tone at 800 Hz for dash
    }

    // If buzzer is active and the duration has elapsed, stop the tone
    if (buzzerActive && currentTime - startTime >= currentDuration) {
        noTone(buzzerPin);  // Turn off buzzer
        buzzerActive = false;  // Reset buzzer state to allow new signals
        delay(gapDuration);  // Short gap before next signal for clarity
    }
}