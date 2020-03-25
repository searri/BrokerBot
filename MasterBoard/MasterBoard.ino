//#include <Wire.h>
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
//  Wire.begin(); // join i2c bus (address optional for master)

  Serial.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);
  lcd.begin(16, 2);
  digitalWrite(LED_BUILTIN, LOW);
}

byte x = 0;

void loop()
{
//  Wire.beginTransmission(4); // transmit to device #4
//  Wire.write(x);              // sends one byte  
//  Wire.endTransmission();    // stop transmitting

  Serial.write(x);

  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(x);
  
  x++;
  delay(1000);
}
