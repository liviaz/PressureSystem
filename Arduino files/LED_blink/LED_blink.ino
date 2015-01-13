
// define globals
int pressurePin = 0;
int pressure = 0;

// setup
void setup()
{
  Serial.begin(9600);
  
}


// loop
void loop()
{
  if(Serial.available())
  {
    pressure = analogRead(pressurePin);
    Serial.print(pressure);
    Serial.flush();
  }
}

  
