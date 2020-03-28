bool welcomePrint = true;
void handleStateZero() {
  if(welcomePrint) {
    lcd.print("Game length:");
    welcomePrint = false;
  }

  // TEMPORARY: will replace with keypad commands
  if(Serial.available()) {
    years = Serial.parseInt();
    lcd.setCursor(0, 1);
    lcd.print(years, DEC);
    lcd.print(" years");
    delay(3000);
    state++;
  }
}
