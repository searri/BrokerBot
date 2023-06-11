void getNumOfYears() {
  lcd.clear();
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
  StaticJsonDocument<200> jsonBuffer;
  
  lcd.clear();
  lcd.print("Connecting to");
  lcd.setCursor(0, 1);
  lcd.print("game server...");

  if((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;

    // Must be defined in "connection.h"
    if (http.begin(client, GAMESERVER_IP)) {
      String stringStub = "{'num_years':";
      String endBracket = "}";
      String toSend = stringStub + (int)years + endBracket;
      
      int httpCode = http.POST(toSend);

      // httpCode will be negative on error
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          deserializeJson(jsonBuffer, payload);
          String temp;
          serializeJson(jsonBuffer["success"], temp);
          if(temp[0] == 't') {
            state++;
          }
        }
        
      } else {
        lcd.clear();
        lcd.print("HTTP request failed:");
        lcd.setCursor(0, 1);
        lcd.print(http.errorToString(httpCode).c_str());
        delay(100);
      }

      http.end();
    }
    
  } else {
    delay(10);
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
