const int WPM = 20;
const int TONE_HZ = 600;

const int ditPin = 2;
const int dahPin = 3;
const int buzzerPin = 9;

unsigned int ditDuration;
bool lastWasDit = false;
String morseBuffer = "";

unsigned long lastInputTime = 0;
const unsigned long letterGap = 1200 / WPM * 3; // Gap to detect end of letter

struct MorseMap {
  const char* code;
  char letter;
};

MorseMap morseTable[] = {
  {".-", 'A'}, {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'}, {".", 'E'},
  {"..-.", 'F'}, {"--.", 'G'}, {"....", 'H'}, {"..", 'I'}, {".---", 'J'},
  {"-.-", 'K'}, {".-..", 'L'}, {"--", 'M'}, {"-.", 'N'}, {"---", 'O'},
  {".--.", 'P'}, {"--.-", 'Q'}, {".-.", 'R'}, {"...", 'S'}, {"-", 'T'},
  {"..-", 'U'}, {"...-", 'V'}, {".--", 'W'}, {"-..-", 'X'}, {"-.--", 'Y'},
  {"--..", 'Z'}, {"-----", '0'}, {".----", '1'}, {"..---", '2'},
  {"...--", '3'}, {"....-", '4'}, {".....", '5'}, {"-....", '6'},
  {"--...", '7'}, {"---..", '8'}, {"----.", '9'}
};

void setup() {
  pinMode(ditPin, INPUT_PULLUP);
  pinMode(dahPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  ditDuration = 1200 / WPM;
  Serial.println("CW Keyer + Manual Decoder Ready");
}

void loop() {
  bool ditPressed = digitalRead(ditPin) == LOW;
  bool dahPressed = digitalRead(dahPin) == LOW;

  if (ditPressed && !dahPressed) {
    sendDit();
    morseBuffer += ".";
    lastInputTime = millis();
  } else if (dahPressed && !ditPressed) {
    sendDah();
    morseBuffer += "-";
    lastInputTime = millis();
  } else if (ditPressed && dahPressed) {
    if (lastWasDit) {
      sendDah();
      morseBuffer += "-";
    } else {
      sendDit();
      morseBuffer += ".";
    }
    lastWasDit = !lastWasDit;
    lastInputTime = millis();
  }

  // If enough time has passed since the last input, decode
  if (morseBuffer.length() > 0 && millis() - lastInputTime > letterGap) {
    char decoded = decodeMorse(morseBuffer);
    Serial.print("        →   ");
    Serial.println(decoded);
    morseBuffer = "";
  }

  delay(10); // small delay for debounce
}

void sendDit() {
  Serial.print(".");
  tone(buzzerPin, TONE_HZ);
  delay(ditDuration);
  noTone(buzzerPin);
  delay(ditDuration); // spacing
}

void sendDah() {
  Serial.print("-");
  tone(buzzerPin, TONE_HZ);
  delay(ditDuration * 3);
  noTone(buzzerPin);
  delay(ditDuration); // spacing
}

char decodeMorse(String code) {
  for (auto &entry : morseTable) {
    if (code.equals(entry.code)) return entry.letter;
  }
  return '?';
}
