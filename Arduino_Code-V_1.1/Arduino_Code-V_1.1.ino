// Define pin connections
const int buzzerPin = 9;      // The pin connected to the buzzer (outputs sound)
const int buttonDot = 2;      // The pin for the dot button (short Morse signal)
const int buttonDash = 3;     // The pin for the dash button (long Morse signal)

// Define Morse code timing (standard 1:3 ratio)
const int dotDuration = 200;  // Dot duration (milliseconds)
const int dashDuration = 600; // Dash duration (milliseconds)
const int gapDuration = 150;  // Gap between signals for better readability

// Define buzzer frequencies for Morse tones
const int DotHz = 800;  // Frequency for dot sound (Hz)
const int DashHz = 800; // Frequency for dash sound (Hz)

// Variables to track buzzer state
bool buzzerActive = false;    // Flag to check if buzzer is currently active
unsigned long startTime = 0;  // Timestamp when the buzzer signal starts
int currentDuration = 0;      // Holds the duration of the current sound

void setup() {
    // Set pin modes for inputs (buttons) and outputs (buzzer)
    pinMode(buzzerPin, OUTPUT);        // Set buzzer pin as output to generate sound
    pinMode(buttonDot, INPUT_PULLUP);  // Configure dot button with pull-up resistor
    pinMode(buttonDash, INPUT_PULLUP); // Configure dash button with pull-up resistor
}

void loop() {
    // Get the current time for non-blocking execution
    unsigned long currentTime = millis();

    // If dot button is pressed and buzzer is not already active, generate dot sound
    if (digitalRead(buttonDot) == LOW && !buzzerActive) {
        buzzerActive = true;            // Set buzzer to active state
        startTime = currentTime;        // Store the time when the signal starts
        currentDuration = dotDuration;  // Set the signal duration to dot timing
        tone(buzzerPin, DotHz);         // Play 800 Hz tone for dot signal
    }

    // If dash button is pressed and buzzer is not already active, generate dash sound
    if (digitalRead(buttonDash) == LOW && !buzzerActive) {
        buzzerActive = true;            // Set buzzer to active state
        startTime = currentTime;        // Store the time when the signal starts
        currentDuration = dashDuration; // Set the signal duration to dash timing
        tone(buzzerPin, DashHz);        // Play 800 Hz tone for dash signal
    }

    // If buzzer is active and the duration has elapsed, stop the tone
    if (buzzerActive && currentTime - startTime >= currentDuration) {
        noTone(buzzerPin);  // Stop the buzzer sound
        buzzerActive = false;  // Reset buzzer state so new signals can be triggered
        delay(gapDuration);  // Small gap between signals for better readability
    }
}