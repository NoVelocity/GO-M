const int STEP_PIN = 2;
const int DIR_PIN = 3;

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  digitalWrite(STEP_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);
}

void loop() {
  digitalWrite(DIR_PIN, HIGH);

  for (int i = 0; i < 200; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(20);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(980);
  }

  delay(1000);
  digitalWrite(DIR_PIN, LOW);

  for (int i = 0; i < 200; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(20);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(980);
  }

  delay(1000);
}
