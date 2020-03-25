//#include <Wire.h>

#define LED_PIN D3

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
//  Wire.begin(D2, D1);
//  Wire.onReceive(receiveEvent);
}

void loop()
{
  if(Serial.available()) {
    int x = Serial.read();
    if (x%2) {
      analogWrite(LED_PIN, 255);
    } else {
      analogWrite(LED_PIN, 0);
    }
  }
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
//void receiveEvent(size_t howMany)
//{
//  int x = Wire.read();
//  if (x%2) {
//    analogWrite(LED_PIN, 255);
//  } else {
//    analogWrite(LED_PIN, 0);
//  }
//}
