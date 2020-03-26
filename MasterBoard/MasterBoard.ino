#include <LiquidCrystal.h>
#define NUM_ITEMS 18

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
byte toSend[NUM_ITEMS];
byte j = 0;

void setup()
{
  Serial.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  lcd.begin(16, 2);
}

void loop()
{
  if(!(j%2)) {
    for(int i=0; i<NUM_ITEMS/2; i++) {
      toSend[i+(NUM_ITEMS/2)] = 1;
    }
  } else {
    for(int i=0; i<NUM_ITEMS/2; i++) {
      toSend[i+(NUM_ITEMS/2)] = 0;
    }
  }
 
  for(int i=0; i<NUM_ITEMS/2; i++) {
    toSend[i] = j;
  }

  for(int i=0; i < NUM_ITEMS; i++) {
    Serial.write(toSend[i]);
  }

  lcd.clear();
  lcd.print(toSend[0]);
  lcd.setCursor(0, 1);
  lcd.print(toSend[NUM_ITEMS/2]);

  j++;
  delay(1000);
}
