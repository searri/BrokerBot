// Update prices in portfolio, write changes to other board
void handleStateSeven() {
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
