//Classroom Noise Monitor 2.0

const int sampleWindow = 2000;       // Sample window width in mS (50 mS = 20Hz)
const int SampleArraySize = 80;       // Max size of sample array
int TeacherMicThreshold = 162.5;      //Threshold used to compare if the teacher is talking or not (Must be tuned)
float ReferenceSoundPower = 0.000002;   //2E-6
int counter3 = 0;
float TeacherAvgSum = 0;
float ThresholdA = 20;        //Threshold between green and yellow
float ThresholdB = 38;        //Threshold between yellow and Red
float constant = 50;        //For future use this is the number we will have to adjust when live signal testing


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
  float StudentLeq = 0;
  float LeqSumStudent = 0;
  float LeqSumTeacher = 0;
  unsigned int Samples = 0;        //represents the sample number that we are on
  float TeacherISPL = 0;
  float StudentISPL[SampleArraySize];
  float LeqTotal = 0;
  float LeqAverage = 0;
  float TeacherSample;          // Raw Quantized Sample from teacher mic
  float StudentSample;          // Raw Quantized Sample from student mic
  unsigned int NumStudentWindows = 0; //The number of windows/times that samples were being taken from the students

  unsigned long startMillis = millis(); // Start of TeacherSample window
  // collect data for 2000 ms or two seconds
  while (millis() - startMillis < sampleWindow)
  {
    TeacherSample = analogRead(0); //TeacherSample from the teacher mic
    TeacherVoltage = abs((TeacherSample * 5) / 1024 - 1.65); //convert teachmic info to voltage

    //Instantaneous Sound pressure level of teacher aka SNR for teacher
    TeacherISPL = 20 * log10(TeacherVoltage / ReferenceSoundPower) + constant; //

    //while the ISPL of the teacher is below a certain level, start taking samples from the student mic

    //keeps track of the number of samples taken in the time that the teacher was not talking
    Samples = 0;
    while (TeacherISPL < TeacherMicThreshold)
    {
      StudentSample = analogRead(1); //TeacherSample from the student mic
      StudentVoltage = abs((StudentSample * 5) / 1024 - 1.65); //convert stumic info to 
      StudentISPL[Samples] = 20 * log10(StudentVoltage / ReferenceSoundPower) + constant; //Instantaneous Sound pressure level of students aka SNR for student

      //Check if the teacher is talking or not
      TeacherSample = analogRead(0);
      TeacherVoltage = abs((TeacherSample * 5) / 1024 - 1.65);
      TeacherISPL = 20 * log10(TeacherVoltage / ReferenceSoundPower) + constant;

      Samples++;
    }
    NumStudentWindows++;
    //find LEQ of student over the window
    for (int i = 0; i < Samples; i++)
    {
      LeqSumStudent += pow(10, StudentISPL[i] / 10);
    }
    //Basically and average over the number of samples
    StudentLeq = 10 * log10(1 / Samples * LeqSumStudent); //Equivalent level at the end of the sample window
    //Store the result for averaging
    LeqTotal += StudentLeq;
  }
  //Average Leq over the 2 second window

  LeqAverage = LeqTotal / NumStudentWindows;
  //LED activation
  if (LeqAverage <= ThresholdA)
  {
    digitalWrite(11, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
  }
  else if ((LeqAverage > ThresholdA) && (LeqAverage <= ThresholdB))
  {
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
  }
  else
  {
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);
  }
 NumStudentWindows = 0;
}
