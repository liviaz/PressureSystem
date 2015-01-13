
#define LED_PIN 5
#define VALVE_PIN 3
#define VENT_PIN 4

// define globals
int pressurePin = 0;
int pressure = 0;
int LEDbright = 0;
int valveVal = 0;
int ventVal = 0;

// setup
void setup()
{
  
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(VALVE_PIN, OUTPUT);
  pinMode(VENT_PIN, OUTPUT);
  
}


// loop
void loop()
{
  if(Serial.available())
  {
    
    // read bytes to determine which pin and value to write
    int pinToWrite = (int) Serial.read();
    
    switch (pinToWrite) {
      case LED_PIN:
        LEDbright = (int) Serial.read();
        analogWrite(LED_PIN, LEDbright);
        break;
      case VALVE_PIN:
        valveVal = (int) Serial.read();
        if (valveVal > 0){
          digitalWrite(VALVE_PIN, HIGH);
        } else {
          digitalWrite(VALVE_PIN, LOW);
        }
        case VENT_PIN:
        ventVal = (int) Serial.read();
        if (ventVal > 0){
          digitalWrite(VENT_PIN, HIGH);
        } else {
          digitalWrite(VENT_PIN, LOW);
        }
        break;
    }
        
  }
  
  pressure = analogRead(pressurePin);
  Serial.print(pressure);
  
  // print char to signify end
  char endChar = 0xFF;
  Serial.print(endChar); 
  
  Serial.flush();
  delay(25);
    
}


