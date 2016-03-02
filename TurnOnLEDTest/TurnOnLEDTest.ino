void setup()
{
  Serial.begin(9600);
  pinMode(11, OUTPUT); //GREEN
  pinMode(12, OUTPUT); //YELLOW
  pinMode(13, OUTPUT); //RED
  analogReference(DEFAULT);
}

void loop() {


  //test to just turn on all the lights
    digitalWrite(11, HIGH);
    delay(1000);
    digitalWrite(11, LOW);
    
    digitalWrite(12, HIGH);
    delay(1000);
    digitalWrite(12, LOW);
    
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    
}
