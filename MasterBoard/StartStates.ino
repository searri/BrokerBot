void handleStateZero() {
  if(printNext) {
    lcd.print("Game length:");
    printNext = false;
  }

  // TEMPORARY: will replace with keypad commands
  if(Serial.available()) {
    years = Serial.parseInt();
    lcd.setCursor(0, 1);
    lcd.print(years, DEC);
    lcd.print(" years");
    delay(3000);
    state++;
    printNext = true;
  }
}

void handleStateOne() {
  if(printNext) {
    lcd.clear();
    lcd.print("Connecting to");
    lcd.setCursor(0, 1);
    lcd.print("game server...");
    printNext = false;
  }
  
  // TEMPORARY: will write years to other board and wait for confirmation
  if(Serial.available()) {
    Serial.flush();
    state++;
    printNext = true;
  }
}
