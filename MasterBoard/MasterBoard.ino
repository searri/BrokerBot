#include <LiquidCrystal.h>
#include "Stocks.h"
#define NUM_ITEMS 18
#define NUM_STOCKS 10

// Set up LiquidCrystal object
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Logistic attributes for communicating with other board
byte toSend[NUM_ITEMS];
byte j = 0;

// Bot player's portfolio
stock portfolio[NUM_STOCKS];

// Internal logistics
char state = 0;
char years;

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

  portfolio[0].dividend = 50;
  portfolio[1].dividend = 10;
  portfolio[3].dividend = 40;
  portfolio[4].dividend = 70;
  portfolio[7].dividend = 20;
  portfolio[8].dividend = 60;
  portfolio[9].dividend = 30;
}


void loop() {
  switch(state) {
    case 0:
      handleStateZero();
      break;
    default:
      lcd.clear();
      lcd.print(state, DEC);
      break;
  }
  delay(100);
}

void showStockName(short nameCode) {
  switch(nameCode) {
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
