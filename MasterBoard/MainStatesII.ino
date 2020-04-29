// Update prices in portfolio
void updateStockPrices() {
  if(!proceed) {
    for(int i=0; i<NUM_STOCKS; i++) {
      portfolio[i].currVal += portfolio[i].amtChange;

      // If stock value goes over $150, it splits
      if(portfolio[i].currVal >= 150) {
        if(portfolio[i].currVal % 2) {
          portfolio[i].currVal /= 2;
          portfolio[i].currVal++;
        } else {
          portfolio[i].currVal /= 2;
        }
        portfolio[i].quantity *= 2;
      }

      // If stock value goes under 0, it resets to $100 and all shares are lost
      if(portfolio[i].currVal <= 0) {
        portfolio[i].currVal = 100;
        portfolio[i].quantity = 0;
      }
    }
  
    lcd.clear();
    lcd.print("Waiting for");
    lcd.setCursor(0, 1);
    lcd.print("game server...");
    proceed = true;
  }

  // TODO: make POST request to server
  delay(3000);
  state++;
  proceed = false;
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
    delay(3000);
    lcd.clear();
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
    lcd.clear();
    lcd.print("Done buying.");
    delay(3000);
    lcd.clear();
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
    lcd.clear();
    lcd.print("Done buying.");
    delay(3000);
    lcd.clear();
  }
}

// Called after we calculate dividends for the final year
void endGame() {
  // Add final stock values to liquidCash
  for(int i=0; i<NUM_STOCKS; i++) {
    liquidCash += (portfolio[i].currVal * portfolio[i].quantity);
    portfolio[i].quantity = 0;
  }
}
