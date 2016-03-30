
float TeacherMicThreshold = 65.5;      //Threshold used to compare if the teacher is talking or not (Must be tuned)
float ReferenceSoundPower = 0.0000259;
float ReferenceSoundPowerStudent = 0.00001;
float constant1 = 0;
float constant2 = 0;
float maxvalue = 0;
const int sampleWindow = 40;       // Sample window width in mS (50 mS = 20Hz)

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
  int counter = 0;
  unsigned long startMillis = millis(); // Start of sample window
  
  while (millis() - startMillis < sampleWindow)
  {
  
  TeacherSample = analogRead(0); //TeacherSample from the teacher mic
//Serial.println(TeacherSample);
  TeacherVoltage = abs((TeacherSample * 5) / 1024 - 2.5); //convert teachmic info to voltage
  TRMS += sq(TeacherVoltage); //convert teachmic info to voltage
  counter++;
//Serial.println(TeacherVoltage);
  //Instantaneous Sound pressure level of teacher aka SNR for teacher
//  TeacherISPL = 20 * log10(TeacherVoltage / ReferenceSoundPower); //
//  Serial.println(TeacherISPL);
  }
  TRMS = sqrt(TRMS/counter);
  TeacherISPL = 20 * log10(TRMS / ReferenceSoundPower);
  Serial.println(TeacherISPL);
  counter=0;
}

  //        printing test
  
//  if (TeacherISPL >= maxvalue)
//  {
//    Serial.println(TeacherISPL);
//    maxvalue = TeacherISPL;
//  }

//  StudentSample = analogRead(1); //TeacherSample from the student mic
//
//  StudentVoltage = abs((StudentSample * 5) / 1024 - 2.5); //convert stumic info to
//  //      Serial.println(StudentVoltage);
//  StudentISPLTest = 20 * log10(StudentVoltage / ReferenceSoundPowerStudent); //Instantaneous Sound pressure level of students aka SNR for student
////  Serial.println(StudentISPLTest);
//  //    if(StudentISPLTest>=maxvalue)
//  //    {
//  //      Serial.println(StudentISPLTest);
//  //      maxvalue = StudentISPLTest;
//  //    }

