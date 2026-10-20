const int STEP_PIN = 2;
const int DIR_PIN = 3;

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  digitalWrite(STEP_PIN, HIGH); // Нет импульса
  digitalWrite(DIR_PIN, HIGH);
}

void loop() {
  for (int direction = 0; direction < 2; direction++) {
    digitalWrite(DIR_PIN, direction ? LOW : HIGH);
    delay(5);

    for (int i = 0; i < 200; i++) {
      digitalWrite(STEP_PIN, LOW);  // Импульс
      delayMicroseconds(20);
      digitalWrite(STEP_PIN, HIGH); // Конец импульса
      delayMicroseconds(980);
    }

    delay(1000);
  }
}
