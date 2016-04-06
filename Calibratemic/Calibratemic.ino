
float TeacherMicThreshold = 65.5;      //Threshold used to compare if the teacher is talking or not (Must be tuned)
float ReferenceSoundPower = 0.0000259;
float constant1 = 12;
float constant2 = 7;
float maxvalue = 8;
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
  float TeacherVoltageArray[sampleWindow];
  float StudentVoltage = 0;
  double StudentLeq = 0;
  float TeacherISPL = 0;
  float StudentISPLTest = 0;
  float TeacherSample;          // Raw Quantized Sample from teacher mic
  float StudentSample;          // Raw Quantized Sample from student mic
  float TRMS = 0;
  float SRMS = 0;
  int counter = 0;
  unsigned long startMillis = millis(); // Start of sample window
  
  while (millis() - startMillis < sampleWindow)
  {
  
  TeacherSample = analogRead(0); //TeacherSample from the teacher mic
//Serial.print(TeacherSample);
//Serial.print("\t");
//Serial.println(millis());
  TeacherVoltage = (TeacherSample * 5) / 1024 - 2.5; //convert teachmic info to voltage
  //Serial.print(TeacherSample);
//Serial.println(TeacherVoltage);
  TRMS += sq(TeacherVoltage); //convert teachmic info to voltage
  counter++;
//
//  StudentSample = analogRead(1); //TeacherSample from the student mic
////  Serial.print(StudentSample);
////Serial.print("\t");
//
////  StudentVoltage = abs((StudentSample * 5) / 1024 - 2.5)*2; //convert stumic info to
//  StudentVoltage = (StudentSample * 5) / 1024 - 2.5; //convert stumic info to
//  SRMS += sq(StudentVoltage); //convert Student info to voltage
////  //      Serial.println(StudentVoltage);
//

  }
  TRMS = sqrt(TRMS/counter);
  Serial.print(TRMS);
Serial.print("\t");

//  SRMS = sqrt(SRMS/counter);
//    Serial.print(SRMS);
//Serial.print("\t");

  TeacherISPL = 20 * log10(TRMS / ReferenceSoundPower) + constant1;
  StudentISPLTest = 20 * log10(SRMS / ReferenceSoundPower) + constant2; //Instantaneous Sound pressure level of students aka SNR for student
  Serial.println(TeacherISPL);
//  Serial.println(StudentISPLTest);
  counter=0;
}



