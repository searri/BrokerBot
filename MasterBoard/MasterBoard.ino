#include <LiquidCrystal.h>
#include "Stocks.h"
#define NUM_STOCKS 10
#define NUM_ITEMS (NUM_STOCKS-1)*2

// Set up LiquidCrystal object
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Bot player's portfolio
stock portfolio[NUM_STOCKS];
int liquidCash = 5000;

// Internal logistics
char state = 0;
char years;
char yearsPassed = 0;
bool printNext = true;
bool isBullMkt;
char priceChanges[NUM_STOCKS];
bool proceed = false;

void setup() {
  // Initialize hardware I/O
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  lcd.begin(16, 2);

  // Initialize all stocks at starting value
  for(int i=0; i<NUM_STOCKS; i++) {
    stock thisStock = {i, 0, 0, 100, 0, 0};
    portfolio[i] = thisStock;
  }

  // Set stock dividend values
  portfolio[0].dividend = 50;
  portfolio[1].dividend = 10;
  portfolio[3].dividend = 40;
  portfolio[4].dividend = 70;
  portfolio[7].dividend = 20;
  portfolio[8].dividend = 60;
  portfolio[9].dividend = 30;

  // Shuffle random seed using data noise on A0
  randomSeed(analogRead(A0));
}


void loop() {
  // Error check
  if(liquidCash < 0) {
    state = 101;
  }

  // Handle current game state
  switch(state) {
    case 0:
      handleStateZero();
      break;
    case 1:
      handleStateOne();
      break;
    case 2:
      handleStateTwo();
      break;
    case 3:
      handleStateThree();
      break;
    case 4:
      handleStateFour();
      break;
    case 5:
      handleStateFive();
      break;
    case 6:
      handleStateSix();
      break;
    case 7:
      handleStateSeven();
      break;
    default:
      lcd.clear();
      lcd.print(state, DEC);
      lcd.print(" - $");
      lcd.print(liquidCash);
      lcd.setCursor(0, 1);
      lcd.print("ERROR STATE");
      break;
  }
  delay(100);
}

// Helper function to purchase stock (needs user interaction)
short purchaseStock(short stockID, short q) {
  int purchasePrice = portfolio[stockID].currVal * q;

  // Check to make sure this transaction can be made
  if(purchasePrice > liquidCash) {
    state = 102;
    return 0;
  } else if (q % 10) {
    state = 103;
    return 0;
  }

  // TEMPORARY: will change to keypad input
  lcd.clear();
  showStockName(stockID);
  lcd.setCursor(0, 1);
  lcd.print("Buy ");
  lcd.print(q);
  while(!Serial.available()){}
  short actuallyPurchased = Serial.parseInt();
  lcd.print(" ");
  lcd.print(actuallyPurchased);
  delay(1500);

  // Update liquid cash and portfolio quantity
  liquidCash -= (actuallyPurchased*portfolio[stockID].currVal);
  portfolio[stockID].quantity += actuallyPurchased;
  portfolio[stockID].startVal = portfolio[stockID].currVal;
  return actuallyPurchased;
}

// Helper function to print stock name on LCD
void showStockName(short stockID) {
  switch(stockID) {
    case 0:
      lcd.print("CITY BONDS");
      break;
    case 1:
      lcd.print("GROWTH CORP");
      break;
    case 2:
      lcd.print("METRO PROP");
      break;
    case 3:
      lcd.print("PIONEER MUTL");
      break;
    case 4:
      lcd.print("SHADY BROOKS");
      break;
    case 5:
      lcd.print("STRYKER");
      break;
    case 6:
      lcd.print("TRI CITY");
      break;
    case 7:
      lcd.print("UNITED AUTO");
      break;
    case 8:
      lcd.print("URANIUM ENT");
      break;
    case 9:
      lcd.print("VALLEY PWR");
      break;
    default:
      lcd.print("ERROR");
      break;
  }
}
