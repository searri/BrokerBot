void getNumOfYears() {
  lcd.print("Game length:");

  // Get encoder input for number of years
  years = getEncoderVal(2, 20, 1);

  // Confirm to players the input
  lcd.setCursor(0, 1);
  lcd.print(years, DEC);
  lcd.print(" years");
  delay(2000);
  state++;
}

void setUpServer() {
  lcd.clear();
  lcd.print("Connecting to");
  lcd.setCursor(0, 1);
  lcd.print("game server...");

  // TODO: make POST request to server
  delay(3000);
  state++;
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
    short stockToBuy = random(1, NUM_STOCKS+1);
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
