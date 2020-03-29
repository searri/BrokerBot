#define RED_LED D3
#define GREEN_LED D4
#define BLUE_LED D5
#define MAX_WAIT 1000
#define NUM_ITEMS 18

void setup()
{
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
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
  
  delay(100);
}

void changeColor(int red, int green, int blue) {
  analogWrite(RED_LED, red);
  analogWrite(GREEN_LED, green);
  analogWrite(BLUE_LED, blue);
}
