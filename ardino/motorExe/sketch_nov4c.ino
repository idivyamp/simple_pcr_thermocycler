int hpin = 8; // Define hpin as a global variable
int cpin = 7; // Define cpin as a global variable
int buz  = 6;


void setup() {
  // put your setup code here, to run once:
  pinMode(hpin, OUTPUT);
  pinMode(cpin, OUTPUT);


  Serial.begin(9600); // Set the baud rate to 9600
  Serial.println("check");
}


void loop() {
  // put your main code here, to run repeatedly:
 
  // Heating (Forward Drive)
  digitalWrite(hpin, HIGH);
  digitalWrite(cpin, LOW); // Ensure the cooling side is OFF
  Serial.println("Heating");
  delay(10000); // Run heating for 10 seconds (or adjust as needed)
 
  // Cooling (Reverse Drive)
  digitalWrite(hpin, LOW); // Turn off the heating side
  digitalWrite(cpin, HIGH);
  Serial.println("Cooling");
  delay(10000); // Run cooling for 10 seconds (or adjust as needed)
  tone(buz, 1000);
    delay(5000);
}



