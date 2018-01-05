void blink_led() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(3, HIGH);
    delay(10);
    digitalWrite(3, LOW);
    delay(10);
  }
}

