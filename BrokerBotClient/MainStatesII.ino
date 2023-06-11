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

  StaticJsonDocument<200> jsonBuffer;
  if((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;

    // Must be defined in "connection.h"
    if (http.begin(client, GAMESERVER_IP)) {
      String stringStub = "{'price_changes':[";
      String toSend = stringStub;
      for(int i=0; i<NUM_STOCKS; i++) {
        toSend += portfolio[i].amtChange;
        toSend += ", ";
      }
      toSend += "], 'year':";
      toSend += (int)currYear;
      toSend += "}";
      
      int httpCode = http.POST(toSend);

      // httpCode will be negative on error
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          deserializeJson(jsonBuffer, payload);
          String temp;
          serializeJson(jsonBuffer["success"], temp);
          if(temp[0] == 't') {
            if(currYear >= years) {
              // These are the final prices; jump to game end
              state = 10;
            } else {
              // Otherwise, attempt to start selling stocks
              state++;
              proceed = false; 
            }
          }
        }
        
      } else {
        lcd.clear();
        lcd.print("Request failed:");
        lcd.setCursor(0,1);
        lcd.print(http.errorToString(httpCode).c_str());
        delay(100);

        // Manual override to avoid getting stuck
        if(getEncoderVal(0, 1, 1)) {
          if(currYear >= years) {
            // These are the final prices; jump to game end
            state = 10;
          } else {
            // Otherwise, attempt to start selling stocks
            state++;
            proceed = false; 
          }
        }
      }

      http.end();
    }
    
  } else {
    delay(10);
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
