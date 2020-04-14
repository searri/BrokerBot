void getNumOfYears() {
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

void setUpServer() {
  if(printNext) {
    lcd.clear();
    lcd.print("Connecting to");
    lcd.setCursor(0, 1);
    lcd.print("game server...");
    printNext = false;
  }
  
  // TEMPORARY: will write years to other board and wait for confirmation
  if(Serial.available()) {
    short a = Serial.parseInt();
    state++;
    printNext = true;
  }
}

// STRATEGY
void purchaseStartStocks() {
  short stocksToBuy[NUM_STOCKS];
  for(int i=0; i<NUM_STOCKS; i++) {
    stocksToBuy[i] = 0;
  }

  // To start game, spend all money
  char stockPurchased = 0;
  while(stockPurchased < 5) {
    short stockToBuy = random(0, NUM_STOCKS+1);
    stocksToBuy[stockToBuy]++;
    stockPurchased++;
  }

  for(int i=0; i<NUM_STOCKS; i++) {
    if(stocksToBuy[i]) {
      short ret = purchaseStock(i, stocksToBuy[i]*10);
      if(stocksToBuy[i]*10 - ret) {
        state = 104;
        return;
      }
    }
  }

  printNext = true;
  state++;
}
