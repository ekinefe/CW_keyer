// Define pin connections
const int buzzerPin = 9;
const int buttonDot = 2;
const int buttonDash = 3;

// Define Morse code timing
const int dotDuration = 200;
const int dashDuration = 600;
const int gapDuration = 150;

// Define buzzer frequencies
const int DotHz = 800;
const int DashHz = 800;

// Buzzer timing variables
bool buzzerActive = false;
unsigned long buzzerStartTime = 0;
int currentDuration = 0;

void setup() {
    pinMode(buzzerPin, OUTPUT);
    pinMode(buttonDot, INPUT_PULLUP);
    pinMode(buttonDash, INPUT_PULLUP);

    Serial.begin(9600);  // Start serial communication
    printMorseTable();   // Print Morse code table at startup
}

void loop() {
    unsigned long currentTime = millis();

    if (digitalRead(buttonDot) == LOW && !buzzerActive) {
        startDot();
    }

    if (digitalRead(buttonDash) == LOW && !buzzerActive) {
        startDash();
    }

    if (buzzerActive && currentTime - buzzerStartTime >= currentDuration) {
        stopBuzzer();
    }
}

// Function to start a dot signal
void startDot() {
    buzzerActive = true;
    buzzerStartTime = millis();  // Get current time dynamically
    currentDuration = dotDuration;
    tone(buzzerPin, DotHz);
}

// Function to start a dash signal
void startDash() {
    buzzerActive = true;
    buzzerStartTime = millis();  // Get current time dynamically
    currentDuration = dashDuration;
    tone(buzzerPin, DashHz);
}

// Function to stop the buzzer after signal duration ends
void stopBuzzer() {
    noTone(buzzerPin);
    buzzerActive = false;
}

// Function to print the full Morse code table to the Serial Monitor
void printMorseTable() {
    Serial.println("\nMorse Code Reference Table:");
    Serial.println("----------------------------");

    Serial.println("\nLetters A-Z:");
    Serial.println("A  -> .-");    Serial.println("B  -> -...");
    Serial.println("C  -> -.-.");  Serial.println("D  -> -..");
    Serial.println("E  -> .");     Serial.println("F  -> ..-.");
    Serial.println("G  -> --.");   Serial.println("H  -> ....");
    Serial.println("I  -> ..");    Serial.println("J  -> .---");
    Serial.println("K  -> -.-");   Serial.println("L  -> .-..");
    Serial.println("M  -> --");    Serial.println("N  -> -.");
    Serial.println("O  -> ---");   Serial.println("P  -> .--.");
    Serial.println("Q  -> --.-");  Serial.println("R  -> .-.");
    Serial.println("S  -> ...");   Serial.println("T  -> -");
    Serial.println("U  -> ..-");   Serial.println("V  -> ...-");
    Serial.println("W  -> .--");   Serial.println("X  -> -..-");
    Serial.println("Y  -> -.--");  Serial.println("Z  -> --..");

    Serial.println("\nNumbers 0-9:");
    Serial.println("0  -> -----"); Serial.println("1  -> .----");
    Serial.println("2  -> ..---"); Serial.println("3  -> ...--");
    Serial.println("4  -> ....-"); Serial.println("5  -> .....");
    Serial.println("6  -> -...."); Serial.println("7  -> --...");
    Serial.println("8  -> ---.."); Serial.println("9  -> ----.");

    Serial.println("\nCommon Symbols:");
    Serial.println(".  -> .-.-.-"); Serial.println(",  -> --..--");
    Serial.println("?  -> ..--.."); Serial.println("!  -> -.-.--");
    Serial.println("/  -> -..-.");  Serial.println("() -> -.--.-");
    Serial.println("&  -> .-...");  Serial.println(":  -> ---...");
    Serial.println("=  -> -...-");

    Serial.println("----------------------------");
    Serial.println("Morse code table printed successfully!");
}