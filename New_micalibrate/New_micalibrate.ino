
float TeacherMicThreshold = 85;      //Threshold used to compare if the teacher is talking or not (Must be tuned)
float ReferenceSoundPower = 0.0000259;
float constant1 = 12;
float constant2 = 7;
float maxvalue = 8;
float constant = 60;
const int sampleWindow = 20;       // Sample window width in mS (50 mS = 20Hz)

void setup()
{
  Serial.begin(9600);
  pinMode(11, OUTPUT); //GREEN
  pinMode(12, OUTPUT); //YELLOW
  pinMode(13, OUTPUT); //RED
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  analogReference(DEFAULT);
}

void loop()
{
  float TeacherVoltage = 0;
  float TeacherVoltage1 = 0;
  float TeacherVoltageArray[sampleWindow];
  float StudentVoltage = 0;
  float StudentVoltage1 = 0;
  double StudentLeq = 0;
  int TeacherISPL = 0;
  float StudentISPLTest = 0;
  int StudentISPLFinal = 0;
  float TeacherSample;          // Raw Quantized Sample from teacher mic
  float StudentSample;          // Raw Quantized Sample from student mic
  float TRMS = 0;
  float TRMS1 = 0;
  float SRMS = 0;
  int counter = 0;
  int counterTeacher = 0;
  int num = 0;
  int accume = 0;
  unsigned long startMillis = millis(); // Start of sample window
  
  while (millis() - startMillis < sampleWindow)
  {
    
    TeacherSample = analogRead(0); //TeacherSample from the teacher mic
    //Serial.print(TeacherSample);
    TeacherVoltage = (TeacherSample * 5) / 1024 - 2.5; //convert teachmic info to voltage
    TRMS += sq(TeacherVoltage); //convert teachmic info to voltage
    counterTeacher++;
  }
  TRMS = sqrt(TRMS/counterTeacher);
  TeacherISPL = 20 * log10(TRMS / ReferenceSoundPower) + constant1;
  Serial.println(TeacherISPL);
  counterTeacher = 0;
  
  //Serial.print(TeacherISPL);
  while (TeacherISPL < TeacherMicThreshold){
      //Serial.print("teacher is talking");
      unsigned long startMillis1 = millis();
      while (millis() - startMillis1 < sampleWindow){
        counter++;
        StudentSample = analogRead(1); //TeacherSample from the student mic
        //  Serial.print(StudentSample);
        //Serial.print("\t");
        StudentVoltage = abs((StudentSample * 5) / 1024 - 2.5)*2; //convert stumic info to
        StudentVoltage = (StudentSample * 5) / 1024 - 2.5; //convert stumic info to
        SRMS += sq(StudentVoltage); //convert Student info to voltage
      }
      SRMS = sqrt(SRMS/counter);
      StudentISPLTest = 20 * log10(SRMS / ReferenceSoundPower) + constant2; //Instantaneous Sound pressure level of students aka SNR for student
      //Serial.println(TeacherISPL);
      StudentISPLTest = ((StudentISPLTest - 30));
      counter=0;
      
      if(StudentISPLTest <= 40){
        Serial.println(pow(StudentISPLTest,1.075));
        StudentISPLFinal = pow(StudentISPLTest,1.075);
        Serial.println("hahahaha");
      }
      else{
        StudentISPLFinal = pow(StudentISPLTest,1.09);
        Serial.println(pow(StudentISPLTest,1.09));
        Serial.println("fafafafa");
      }
      
       if(StudentISPLFinal <= 65){
          digitalWrite(11, HIGH);
          digitalWrite(12, LOW);
          digitalWrite(13, LOW);
          delay(750);
       }
       else if ((StudentISPLFinal > 65) && (StudentISPLFinal < 70)){
          digitalWrite(11, LOW);
          digitalWrite(12, HIGH);
          digitalWrite(13, LOW);
          delay(750);
       }
       else{
          digitalWrite(11, LOW);
          digitalWrite(12, LOW);
          digitalWrite(13, HIGH);
          delay(750);
       }
      /*while(num<200){
        num++;
        accume += StudentISPLTest;
      }
  
      if((accume/200) < 40){
        Serial.println(pow((accume/200),1.075));
        StudentISPLFinal = pow((accume/200),1.075);
        Serial.println("hahahaha");
      }
      else{
        StudentISPLFinal = pow((accume/200),1.09);
        Serial.println(pow((accume/200),1.09));
        Serial.println("fafafafa");
      }*/
      unsigned long startMillis2 = millis();
      while (millis() - startMillis2 < sampleWindow){
        TeacherSample = analogRead(0); //TeacherSample from the teacher mic

        TeacherVoltage1 = (TeacherSample * 5) / 1024 - 2.5; //convert teachmic info to voltage

        TRMS1 += sq(TeacherVoltage1); //convert teachmic info to voltage
        counterTeacher++;
      }
      TRMS1 = sqrt(TRMS1/counterTeacher++);
      TeacherISPL = 20 * log10(TRMS1 / ReferenceSoundPower) + constant1;
      Serial.print(TeacherISPL);
      Serial.println("xxxxxxx");
      counterTeacher = 0;
      //Serial.print(TeacherISPL);
  }

  //TRMS = sqrt(TRMS/counter);
  //Serial.print(TRMS);
//Serial.print("\t");

  //SRMS = sqrt(SRMS/counter);
   // Serial.print(SRMS);
//Serial.print("\t");

  //TeacherISPL = 20 * log10(TRMS / ReferenceSoundPower) + constant1;

  


  /*while(num<200){
    
    num++;
    accume += StudentISPLTest;
  }
  Serial.println(accume/200);*/
}



