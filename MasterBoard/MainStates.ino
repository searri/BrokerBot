void handleStateThree() {
  // Calculate dividends on current portfolio
  for(int i=0; i<NUM_STOCKS; i++) {
    liquidCash += (portfolio[i].dividend * (portfolio[i].quantity/10));
  }
  state++;
}

void handleStateFour() {
  if(printNext) {
    lcd.clear();
    lcd.print("ODD: Bull");
    lcd.setCursor(0, 1);
    lcd.print("EVEN: Bear");
    printNext = false;
  }

  // TEMPORARY: will replace with keypad commands
  if(Serial.available()) {
    char roll = Serial.parseInt();
    lcd.clear();
    lcd.print(roll, DEC);
    lcd.print(" : ");
    if(roll % 2) {
      lcd.print("BULL");
      isBullMkt = true;
    } else {
      lcd.print("BEAR");
      isBullMkt = false;
    }
    delay(3000);
    state++;
    printNext = true;
  }
}
