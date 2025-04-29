// Define pin connections
const int buzzerPin = 9;      // The pin connected to the buzzer
const int buttonDot = 2;      // The pin for the dot button
const int buttonDash = 3;     // The pin for the dash button
const int powerLed = 8;     // The pin for power led
const int potPin = A0;        // The pin connected to the potentiometer (analog input)

// Define default Morse code timing
int dotDuration = 52;  // Duration of a dot in milliseconds (200 ms recomanded)
int dashDuration = 156; // Duration of a dash in milliseconds (600 ms recomanded)
const int gapDuration = 150;  // Short gap between signals

// Define buzzer frequencies

const int DotHz = 800;  // Frequency for dot sound
const int DashHz = 800; // Frequency for dash sound

// Timing variables
bool buzzerActive = false;       // Tracks if the buzzer is currently active
unsigned long buzzerStartTime = 0; // Stores start time for buzzer
int currentDuration = 0;         // Stores duration of the current sound

void setup() {
    digitalWrite(powerLed, HIGH);
    pinMode(buzzerPin, OUTPUT);
    pinMode(buttonDot, INPUT_PULLUP);
    pinMode(buttonDash, INPUT_PULLUP);
    Serial.begin(9600); // Initialize serial communication for debugging
}

void loop() {
    unsigned long currentTime = millis(); // Get the current time

    // Read the potentiometer value (0 to 1023) and map it to a desired range for speed
    int potValue = analogRead(potPin);  // Read potentiometer value (0 to 1023)
    int speedFactor = map(potValue, 0, 1023, 100, 1000); // Map the potentiometer to a suitable range for speed

    // Adjust dot and dash duration based on the potentiometer value
    dotDuration = speedFactor;  // Update dot duration
    dashDuration = speedFactor * 3;  // Update dash duration (generally dash is longer than dot)

    // Debugging output to check the mapped values
    // Serial.print("Potentiometer value: ");
    // Serial.print(potValue);
    // Serial.print(" | Speed Factor: ");
    // Serial.println(speedFactor);

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
