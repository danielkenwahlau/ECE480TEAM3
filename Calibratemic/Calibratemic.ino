
float TeacherMicThreshold = 65.5;      //Threshold used to compare if the teacher is talking or not (Must be tuned)
float ReferenceSoundPower = 0.00025;
float ReferenceSoundPowerStudent = 0.00001;
float ThresholdA = 36;        //Threshold between green and yellow
float ThresholdB = 39;        //Threshold between yellow and Red
float constant1 = -27;
float constant2 = 0;
float maxvalue = 0;

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
  float StudentVoltage = 0;
  double StudentLeq = 0;
  double LeqSumStudent = 0;
  float LeqSumTeacher = 0;
  unsigned int Samples = 0;        //represents the sample number that we are on
  float TeacherISPL = 0;
  float StudentISPLTest = 0;
  float StudentISPL[SampleArraySize];
  double LeqTotal = 0;
  float LeqAverage = 0;
  float TeacherSample;          // Raw Quantized Sample from teacher mic
  float StudentSample;          // Raw Quantized Sample from student mic
  unsigned int NumStudentWindows = 0; //The number of windows/times that samples were being taken from the students}

  TeacherSample = analogRead(0); //TeacherSample from the teacher mic

  TeacherVoltage = abs((TeacherSample * 5) / 1024 - 2.6); //convert teachmic info to voltage

  //Instantaneous Sound pressure level of teacher aka SNR for teacher
  TeacherISPL = 20 * log10(TeacherVoltage / ReferenceSoundPower); //
  //        printing test
  Serial.println(TeacherISPL);
//  if (TeacherISPL >= maxvalue)
//  {
//    Serial.println(TeacherISPL);
//    maxvalue = TeacherISPL;
//  }

  StudentSample = analogRead(1); //TeacherSample from the student mic

  StudentVoltage = abs((StudentSample * 5) / 1024 - 2.5); //convert stumic info to
  //      Serial.println(StudentVoltage);
  StudentISPLTest = 20 * log10(StudentVoltage / ReferenceSoundPowerStudent); //Instantaneous Sound pressure level of students aka SNR for student
//  Serial.println(StudentISPLTest);
  //    if(StudentISPLTest>=maxvalue)
  //    {
  //      Serial.println(StudentISPLTest);
  //      maxvalue = StudentISPLTest;
  //    }

}
