const int WPM = 20;
const int TONE_HZ = 600;

const int ditPin = 2;
const int dahPin = 3;
const int buzzerPin = 9;

unsigned int ditDuration;
bool lastWasDit = false;

void setup() {
  pinMode(ditPin, INPUT_PULLUP);
  pinMode(dahPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  ditDuration = 1200 / WPM;
  Serial.println("Iambic CW Keyer Ready");
}

void loop() {
  bool ditPressed = digitalRead(ditPin) == LOW;
  bool dahPressed = digitalRead(dahPin) == LOW;

  if (ditPressed && !dahPressed) {
    sendDit();
  } else if (dahPressed && !ditPressed) {
    sendDah();
  } else if (ditPressed && dahPressed) {
    // Iambic Mode A: alternate
    if (lastWasDit) {
      sendDah();
    } else {
      sendDit();
    }
    lastWasDit = !lastWasDit;
  }
  // Allow time between elements
  delay(ditDuration);
}

void sendDit() {
  Serial.println("Dit");
  tone(buzzerPin, TONE_HZ);
  delay(ditDuration);
  noTone(buzzerPin);
}

void sendDah() {
  Serial.println("Dah");
  tone(buzzerPin, TONE_HZ);
  delay(ditDuration * 3);
  noTone(buzzerPin);
}
