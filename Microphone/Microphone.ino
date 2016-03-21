// Arduino pin numbers
const int DO_pin = 2;
const int AO_pin = 0;
int sound;


 
void setup() {
  pinMode(DO_pin, INPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}
 
void loop() {
    if (sound < 500) {
    digitalWrite(11, HIGH);
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
  }
    if (sound > 600) {
    digitalWrite(11, LOW);
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
  }
    if (sound > 700) {
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);
  }
  
  sound = analogRead(AO_pin);
  Serial.print(digitalRead(DO_pin));
  Serial.print("-");
  Serial.println(analogRead(AO_pin));

 
  
}
