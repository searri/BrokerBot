// Update prices in portfolio, write changes to other board
void updateStockPrices() {
  if(!proceed) {
    for(int i=0; i<NUM_STOCKS; i++) {
      portfolio[i].currVal += portfolio[i].amtChange;
    }
  
    byte toSend[NUM_ITEMS];
    for(int i=0; i<NUM_ITEMS; i++) {
      if(portfolio[i+1].amtChange < 0) {
        toSend[i] = portfolio[i+1].amtChange*-1;
        toSend[i+(NUM_ITEMS/2)] = 1;
      } else {
        toSend[i] = portfolio[i+1].amtChange;
        toSend[i+(NUM_ITEMS/2)] = 0;
      }
    }
  
    lcd.clear();
    lcd.print("Waiting for");
    lcd.setCursor(0, 1);
    lcd.print("game server...");
    proceed = true;
  }

  // TODO: write toSend to the other board
  // TEMPORARY: will write to other board and wait for confirmation
  if(Serial.available()) {
    short a = Serial.parseInt();
    state++;
    proceed = false;
  }
}

// STRATEGY
void findStocksToSell() {
  bool soldAny = false;
  
  // Find stocks that have increased at least this threshold
  short sellThreshold = 20;

  // ...and then sell all of them
  for(int i=0; i<NUM_STOCKS; i++) {
    if(portfolio[i].currVal - portfolio[i].startVal >= sellThreshold && portfolio[i].quantity) {
      sellStock(i, portfolio[i].quantity);
      soldAny = true;
    }
  }

  if(!soldAny) {
    lcd.clear();
    lcd.print("Not selling.");
    lcd.setCursor(0, 1);
    lcd.print("Continue: any #");
    while(!Serial.available()){}
    short actuallySold = Serial.parseInt();
    lcd.clear();
    delay(1000);
  }

  state++;
}

// STRATEGY
void findStocksToBuy() {
  // Don't buy bonds
  portfolio[0].canBuy = false;
  
  // Stock prices can never be over 150 a share
  short minVal = 151;
  short minInd = -1;
  for(short i=0; i<NUM_STOCKS; i++) {
    if(portfolio[i].currVal < minVal && portfolio[i].canBuy) {
      minVal = portfolio[i].currVal;
      minInd = i;
    }
  }

  // We now have the cheapest stock -- invest as much as possible
  short quantToBuy = liquidCash/(minVal*10);
  // If this is 0, we can't afford even the cheapest stock
  if(!quantToBuy) {
    state = 3;
    return;
  }

  // Note: if we already own some of this stock, the original price will be lost
  short actuallyBought = purchaseStock(minInd, quantToBuy*10);

  if(actuallyBought < quantToBuy*10) {
    // There's still some money leftover, try buying something else
    portfolio[minInd].canBuy = false;
  } else {
    // We successfully bought everything, move to next year
    state = 3;
  }
}
