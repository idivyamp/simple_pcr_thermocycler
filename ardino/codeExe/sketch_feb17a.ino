#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin configurations
int pinotermistor = A0;
int tempup = 6, tempdown = 5, cup = 8, hpin = 7, cpin = 9, buz = 10;

// Temperature variables
int Temp, T = 24, mV, settemp = 50;
int denat, annel, exten; // Default Temperatures for the process

// Control variables
int debounceDelay = 100;
unsigned long previousMillis = 0, interval = 0;
int currentStep = 0, i;
int fla1 = 1, fla2 = 1, fla3 = 1, fla4 = 1;

// Enumeration for different process states
enum ProcessState { DENATURATION, ANNEALING, EXTENSION, IDLE };
ProcessState currentProcess = IDLE;

void setup() {
  // Initialize LCD and set up pin modes
  lcd.init();
  lcd.backlight();
  pinMode(tempup, INPUT_PULLUP);
  pinMode(tempdown, INPUT_PULLUP);
  pinMode(cup, INPUT_PULLUP);
  pinMode(hpin, OUTPUT);
  pinMode(cpin, OUTPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
  Serial.println("check");
}

// Function to print setting message on LCD
void printSettingMessage() {
  lcd.setCursor(0, 0);
  lcd.print("Enter ");
  if (currentSetting == 0) lcd.print("D = ");
  else if (currentSetting == 1) lcd.print("A = ");
  else if (currentSetting == 2) lcd.print("E = ");
}

// Function to print cycle message on LCD
void printCycleMessage() {
  lcd.setCursor(0, 1);
  lcd.print("Cycles: " + String(cycleCount));
}

// Function to print cycle count on LCD
void printCycleCount() {
  lcd.setCursor(0, 0);
  lcd.print("Cycle " + String(currentCycle + 1) + " of " + String(cycleCount));
  lcd.setCursor(0, 1);
  lcd.print("Process: " + getProcessName(currentProcess));
}

// Function to get process name as a string
String getProcessName(ProcessState state) {
  switch (state) {
    case DENATURATION: return "Denaturation";
    case ANNEALING: return "Annealing";
    case EXTENSION: return "Extension";
    case IDLE: return "Idle";
  }
  return "Unknown";
}

void loop() {

  // Read temperature from thermistor
  Temp = analogRead(pinotermistor);
  mV = map(Temp, 0, 1023, 0, 5000);
  T = (mV / 10) - 50;

  // Read button states
  int tempupState = digitalRead(tempup);
  int tempdownState = digitalRead(tempdown);
  int cupState = digitalRead(cup);

  // Temperature adjustment
  if (tempupState == LOW && cupState == HIGH) {
    settemp++;
    lcd.setCursor(10, 0);
    lcd.print(settemp);
    Serial.println(settemp);
    delay(debounceDelay);
  }

  if (tempdownState == LOW) {
    settemp--;
    lcd.setCursor(10, 0);
    lcd.print(settemp);
    Serial.println(settemp);
    delay(debounceDelay);
  }

  // Process setting adjustment
  if (tempupState == LOW && tempdownState == LOW) {
    currentSetting = (currentSetting + 1) % 3;
    if (currentSetting == 0 && fla1 == 1) {
      denat = settemp;
      Serial.println("Denat Temp: " + String(denat) + "°C");
      fla1 = 2;
    } else if (currentSetting == 1 && fla2 == 1) {
      annel = settemp;
      Serial.println("Annel Temp: " + String(annel) + "°C");
      fla2 = 2;
    } else if (currentSetting == 2 && fla3 == 1) {
      exten = settemp;
      Serial.println("Exten Temp: " + String(exten) + "°C");
      fla3 = 2;
      currentCycle = 0; // Reset the cycle count when the extension temperature is set
    }
    printSettingMessage();
  }

  // Start the PCR process
  if (tempdownState == LOW && cupState == LOW) {
    lcd.clear();
    Serial.println("started");
    currentSetting = 4;
    printCycleMessage();
    digitalWrite(hpin, HIGH);
    digitalWrite(cpin, LOW);
    delay(15000);

    // Execute PCR cycles
    if (currentCycle <= 0 && currentCycle <= cycleCount) {
      for (i = 0; i <= cycleCount; i++) {
        
      }
    }
    
    // PCR completed message and buzzer alert
    lcd.setCursor(0, 0);
    lcd.print("PCR COMPETED");
    lcd.setCursor(0, 1);
    lcd.print("Wait for coolin");
    delay(30000);
    lcd.setCursor(0, 0);
    lcd.print("PCR COMPETED");
    tone(buz, 1000);
    delay(5000);
  }
}
