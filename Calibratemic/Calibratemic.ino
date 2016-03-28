
int incomingAudio;

void setup(){
  for (byte i=0;i<8;i++){
    pinMode(i,OUTPUT);//set digital pins 0-7 as outputs (DAC)
  }
}

void loop(){
  incomingAudio = analogRead(A0);//read voltage at A0
  incomingAudio = (incomingAudio+1)/4 - 1;//scale from 10 bit (0-1023) to 8 bit (0-255)
  if (incomingAudio<0){//deal with negative numbers
    incomingAudio = 0;
  }
  Serial.println(incomingAudio);
  PORTD = incomingAudio;
}

