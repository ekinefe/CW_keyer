const int buzzerPin = 9;      // Pin connected to the buzzer
const int buttonDot = 2;      // Pin for the dot button
const int buttonDash = 3;     // Pin for the dash button
const int dotDuration = 200;  // Duration of a dot (milliseconds)
const int dashDuration = 600; // Duration of a dash (milliseconds)

void setup() {
    pinMode(buzzerPin, OUTPUT);
    pinMode(buttonDot, INPUT_PULLUP);
    pinMode(buttonDash, INPUT_PULLUP);
}

void loop() {
    if (digitalRead(buttonDot) == LOW) { // If the dot button is pressed
        tone(buzzerPin, 1000);  // Generate sound at 1000 Hz
        delay(dotDuration);
        noTone(buzzerPin);
        delay(dotDuration);  // Short pause before next signal
    }

    if (digitalRead(buttonDash) == LOW) { // If the dash button is pressed
        tone(buzzerPin, 1000);
        delay(dashDuration);
        noTone(buzzerPin);
        delay(dotDuration);  // Short pause before next signal
    }
}