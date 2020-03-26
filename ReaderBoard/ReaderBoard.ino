#define LED_PIN D3
#define MAX_WAIT 1000
#define NUM_ITEMS 18

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

byte rawData[NUM_ITEMS];
int priceChanges[(int)(NUM_ITEMS/2)];
void loop()
{
  // Wait for all Serial data to be available
  while(Serial.available() < NUM_ITEMS) {}
  for(int i=0; i < NUM_ITEMS; i++) {
    rawData[i] = Serial.read(); 
  }

  // Decode serial data into negative and positive values
  for(int i=0; i < NUM_ITEMS/2; i++) {
    priceChanges[i] = rawData[i];
    if(rawData[i + NUM_ITEMS/2]) {
      priceChanges[i] *= -1;
    }
  }

  // TEMPORARY DEBUG CODE
  if(priceChanges[0] < 0) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  
  delay(100);
}
