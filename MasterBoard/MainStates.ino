void calcDividends() {  
  // Calculate dividends on current portfolio
  for(int i=0; i<NUM_STOCKS; i++) {

    // Stock only yields dividends if its value is over 50
    if(portfolio[i].currVal > 50) {
      liquidCash += (portfolio[i].dividend * (portfolio[i].quantity/10)); 
    }
  }

  // Reset purchasability of all stocks
  for(int i=0; i<NUM_STOCKS; i++) {
    portfolio[i].canBuy = true;
  }

  // Advance time
  currYear++;
  state++;
}

void bullOrBear() {
  if(printNext) {
    lcd.clear();
    lcd.print("Roll: BULL/BEAR");
    printNext = false;
  }

  // Get dice roll for Bull/Bear market
  int roll = getEncoderVal(1, 6, 1);
  lcd.setCursor(0, 1);
  lcd.print(roll);
  lcd.print(": ");
  if(roll % 2) {
    lcd.print("BULL");
    isBullMkt = true;
  } else {
    lcd.print("BEAR");
    isBullMkt = false;
  }
  delay(2000);
  state++;
  printNext = true;
}

// Calculate price changes based on dice roll
void getPriceRoll() {
  int roll = 13;
  
  if(printNext) {
    lcd.clear();
    lcd.print("Input dice roll:");
    lcd.setCursor(0, 1);
    printNext = false;
  }

  // Get price change dice roll
  roll = getEncoderVal(2, 12, 1);
  lcd.setCursor(0, 1);
  lcd.print(roll);
  delay(2000);


  // ALL THE PRICE CHANGE DATA
  short bullMktData[11][NUM_STOCKS-1] = {
    {-2, -10,  -7,  -9,  -2,  -9,  -7, -16,  -4},
    {26,  16,  25,   8, -14,  21,  14,  -4,  17},
    {18,  23,  11,  12,  46,  18,  -5,  34,  15},
    {23,  28,  -2,  11,  56,  19,  30,  29,  14},
    {20,  15,  15,   7, -20,  15,  13, -10,  12},
    {17,  21,  13,  -2,  37,  23,  23,  19,  14},
    {19,  24,  17,   9,  -5,  26,  13,  -7,  15},
    {11,  18,  14,  11,  67,  15,  22,  18,  13},
    {13,  31,   1,  14, -11,  18,  18, -14,  10},
    {14,  -8,  19,  -1,  -9,  25, -10,  13,  19},
    {24,  24,  23,  20,  51,  27,  38,  33,  18}
  };

  short bearMktData[11][NUM_STOCKS-1] = {
    {12,  14,  13,  10,  10,  20,  21,  25,  8},
    { 7,  -6,  10, -10,  30,   6, -19,  22, -2},
    { 9,  10,   7,  -5, -20,  12,  21,  18,  7},
    { 7,   8,   5,  -6, -40,   3,  16, -14,  4},
    { 8,   6,   4,  -4,  40,   8,   4, -12,  3},
    { 6,   4,   3,   3, -15,   5,   8,  -8,  5},
    { 5,   7,  -1,  -3,  45,   6, -10,  10,  4},
    {-2,   6,  -3,  -8, -20,   7,  10,  14,  6},
    {11,  11,  -5,  -7,  30,  10, -11, -18, -4},
    {-5,  13,  -8,   6,  25,   4,  18, -22, -4},
    {-8, -10, -10, -15, -20, -20, -23, -25, -7}
  };

  for(int i=0; i<NUM_STOCKS-1; i++) {
    if(isBullMkt) {
      portfolio[i+1].amtChange = bullMktData[roll-2][i];
    } else {
      portfolio[i+1].amtChange = bearMktData[roll-2][i];
    }
  }

  state++;
  printNext = true;
  proceed = false;
}

// Calculate prices based on event card
void getEventCard() {
  int event = 0;
  if(printNext) {
    lcd.clear();
    lcd.print("Input card code:");
    printNext = false;
  }

  // Get event card from encoder
  event = getEncoderVal(1, 36, 1);
  lcd.setCursor(0, 1);
  lcd.print(event);
  delay(2000);

  switch(event) {
    case 1:
      portfolio[7].amtChange += 10;
      break;
    case 2:
      portfolio[6].amtChange += 5;
      break;
    case 3:
      portfolio[3].amtChange += 3;
      portfolio[9].amtChange += 4;
      break;
    case 4:
      portfolio[1].amtChange += 8;
      portfolio[2].amtChange += 5;
      portfolio[3].amtChange += 5;
      portfolio[7].amtChange += 7;
      break;
    case 5:
      portfolio[8].amtChange += 10;
      break;
    case 6:
      portfolio[7].amtChange -= 5;
      break;
    case 7:
      portfolio[1].amtChange -= 10;
      break;
    case 8:
      portfolio[1].amtChange -= 8;
      portfolio[2].amtChange -= 5;
      portfolio[7].amtChange -= 7;
      break;
    case 9:
      portfolio[9].amtChange += 5;
      break;
    case 10:
      portfolio[6].amtChange -= 5;
      break;
    case 11:
      portfolio[8].amtChange -= 25;
      break;
    case 12:
      portfolio[7].amtChange -= 15;
      break;
    case 13:
      portfolio[2].amtChange += 10;
      break;
    case 14:
      portfolio[4].amtChange -= 5;
      break;
    case 15:
      portfolio[8].amtChange += 10;
      break;
    case 16:
      portfolio[6].amtChange += 10;
      break;
    case 17:
      portfolio[7].amtChange -= 15;
      break;
    case 18:
      portfolio[6].amtChange += 15;
      break;
    case 19:
      portfolio[4].amtChange += 5;
      break;
    case 20:
      portfolio[3].amtChange -= 8;
      portfolio[5].amtChange += 8;
      portfolio[8].amtChange += 5;
      break;
    case 21:
      portfolio[7].amtChange += 15;
      break;
    case 22:
      portfolio[2].amtChange -= 5;
      break;
    case 23:
      portfolio[5].amtChange -= 10;
      break;
    case 24:
      portfolio[1].amtChange -= 10;
      break;
    case 25:
      portfolio[2].amtChange += 5;
      break;
    case 26:
      portfolio[1].amtChange += 8;
      break;
    case 27:
      portfolio[9].amtChange -= 14;
      break;
    case 28:
      portfolio[9].amtChange += 5;
      break;
    case 29:
      portfolio[5].amtChange -= 15;
      break;
    case 30:
      portfolio[5].amtChange += 17;
      break;
    case 31:
      portfolio[6].amtChange -= 25;
      break;
    case 32:
      portfolio[2].amtChange -= 10;
      break;
    case 33:
      portfolio[1].amtChange += 10;
      break;
    case 34:
      portfolio[1].amtChange -= 8;
      break;
    case 35:
      portfolio[7].amtChange += 10;
      break;
    case 36:
      portfolio[1].amtChange += 10;
      liquidCash += (2*portfolio[1].quantity);
      break;
    default:
      state = 105;
      break;
  }

  state++;
  printNext = true;
  proceed = false;
}
