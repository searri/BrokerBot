#include <LiquidCrystal.h>
#include "Stocks.h"
#define NUM_STOCKS 10
#define ENC_CLK D1
#define ENC_SW D2

// Set up LiquidCrystal object
LiquidCrystal lcd(D7, D6, D5, D4, D3, D0);

// Bot player's portfolio
stock portfolio[NUM_STOCKS];
int liquidCash = 5000;

// Internal logistics
char state = 0;
char years = 100;
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

  // Set encoder pins as inputs
  pinMode(ENC_CLK,INPUT);
  pinMode(ENC_SW, INPUT_PULLUP);

  // Initialize all stocks at starting value
  for(int i=0; i<NUM_STOCKS; i++) {
    stock thisStock = {i, 0, 0, 100, 0, 0, true};
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

  // Shuffle random seed using electrical noise on A0
  randomSeed(analogRead(A0));
}


void loop() {
  // Error check
  if(liquidCash < 0) {
    state = 101;
  }

  // Check for game end
  if(yearsPassed >= years) {
    state = 10;
  }

  // Handle current game state
  switch(state) {
    case 0:
      getNumOfYears();
      break;
    case 1:
      setUpServer();
      break;
    case 2:
      purchaseStartStocks();
      break;
    case 3:
      calcDividends();
      break;
    case 4:
      bullOrBear();
      break;
    case 5:
      getPriceRoll();
      break;
    case 6:
      getEventCard();
      break;
    case 7:
      updateStockPrices();
      break;
    case 8:
      findStocksToSell();
      break;
    case 9:
      findStocksToBuy();
      break;
    case 10:
      if(printNext) {
        endGame();
        lcd.clear();
        lcd.print("Good game!");
        lcd.setCursor(0, 1);
        lcd.print("$");
        lcd.print(liquidCash);
        printNext = false;
      }
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

  // Request purchase of stock
  lcd.clear();
  showStockName(stockID);
  lcd.print(": ");
  lcd.print("Buy ");
  lcd.print(q);
  short actuallyPurchased = getEncoderVal(0, q, 10);
  lcd.print(" BOUGHT");
  delay(1500);

  // Update liquid cash and portfolio quantity
  liquidCash -= (actuallyPurchased*portfolio[stockID].currVal);
  portfolio[stockID].quantity += actuallyPurchased;
  portfolio[stockID].startVal = portfolio[stockID].currVal;
  return actuallyPurchased;
}

// Helper function to sell stock (needs user interaction)
void sellStock(short stockID, short q) {
  int sellPrice = portfolio[stockID].currVal * q;

  // Check to make sure this transaction can be made
  if(portfolio[stockID].quantity < q) {
    state = 110;
    return;
  } else if (q % 10) {
    state = 111;
    return;
  }

  // Request selling of stock
  lcd.clear();
  showStockName(stockID);
  lcd.print(": ");
  lcd.print("Sell ");
  lcd.print(q);
  short actuallySold = getEncoderVal(0, q, 10);
  lcd.print(" SOLD");
  delay(1500);

  // Update liquid cash and portfolio quantity
  liquidCash += (q*portfolio[stockID].currVal);
  portfolio[stockID].quantity -= q;
}

// Helper function to get a value from the encoder
// This is like a mini "main" loop that iterates until encoder is clicked
// Will print values on the second line of the LCD
// Adapted from this tutorial: https://lastminuteengineers.com/rotary-encoder-arduino-tutorial/
int getEncoderVal(int minAllowable, int maxAllowable, int stepSize) {
  bool buttonPressed = false;
  int tempInput = minAllowable;
  unsigned long lastButtonPress = 0;
  int currentStateCLK;
  int lastStateCLK;
  
  // Read the initial state of encoder CLK
  lastStateCLK = digitalRead(ENC_CLK);

  // Print current selected value
  lcd.setCursor(0, 1);
  lcd.print("      ");
  lcd.setCursor(0, 1);
  lcd.print(tempInput);
  
  while(!buttonPressed) {   
    // Read the current state of CLK
    currentStateCLK = digitalRead(ENC_CLK);
  
    // If last and current state of CLK are different, then pulse occurred
    // React to only 1 state change to avoid double count
    if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
      // The board physically doesn't have enough pins for a direction wire
      // Thus, either way makes an increase; values wrap around
      tempInput += stepSize;

      if(tempInput > maxAllowable) {
        tempInput = minAllowable;
      }
      
      lcd.setCursor(0, 1);
      lcd.print("      ");
      lcd.setCursor(0, 1);
      lcd.print(tempInput);
    }
  
    // Remember last CLK state
    lastStateCLK = currentStateCLK;
  
    // Read the button state
    int btnState = digitalRead(ENC_SW);
  
    //If we detect LOW signal, button is pressed
    if (btnState == LOW) {
      //if 50ms have passed since last LOW pulse, it means that the
      //button has been pressed, released and pressed again
      if (millis() - lastButtonPress > 50) {
        buttonPressed = true;
      }
  
      // Remember last button press event
      lastButtonPress = millis();
    }
  
    // Put in a slight delay to help debounce the reading
    delay(1);
  }

  return tempInput;
}

// Helper function to print stock name on LCD (6 characters)
void showStockName(short stockID) {
  switch(stockID) {
    case 0:
      lcd.print("CITY  ");
      break;
    case 1:
      lcd.print("GROWTH");
      break;
    case 2:
      lcd.print("METRO ");
      break;
    case 3:
      lcd.print("PIONR ");
      break;
    case 4:
      lcd.print("SHADY ");
      break;
    case 5:
      lcd.print("STRYKR");
      break;
    case 6:
      lcd.print("TRI   ");
      break;
    case 7:
      lcd.print("UNITED");
      break;
    case 8:
      lcd.print("URNIUM");
      break;
    case 9:
      lcd.print("VALLEY");
      break;
    default:
      lcd.print("ERROR ");
      break;
  }
}
