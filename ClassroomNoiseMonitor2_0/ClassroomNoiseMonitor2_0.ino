//Classroom Noise Monitor 2.0


int counterHistory;
int historyArray[25];
int historySum;
const int sampleWindow = 2000;       // Sample window width in mS (50 mS = 20Hz)
const int SampleArraySize = 80;       // Max size of sample array
float TeacherSample;          // Raw Quantized Sample from teacher mic
float StudentSample;          // Raw Quantized Sample from student mic
int TeacherMicThreshold = 30;      //Threshold used to compare if the teacher is talking or not (Must be tuned)
unsigned int Samples;        //represents the sample number that we are on
float Snr;
//float TeacherISPL[SampleArraySize];
//float StudentISPL[SampleArraySize];
float TeacherISPL;
float StudentISPL[SampleArraySize];

float ReferenceSoundPower = 0.000002;   //2E-6
int counter3 = 0;
float TeacherAvgSum = 0;


void setup()
{
  Serial.begin(9600);
  pinMode(11, OUTPUT); //GREEN
  pinMode(12, OUTPUT); //YELLOW
  pinMode(13, OUTPUT); //RED
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  analogReference(DEFAULT);
}
void loop()
{

  float TeacherVoltage;
  float StudentVoltage;
  float TeacherLeq;
  float StudentLeq;

  float TeacherSummation = 0;
  float StudentSummation = 0;
  float LeqSumStudent = 0;
  float LeqSumTeacher = 0;
  unsigned long startMillis = millis(); // Start of TeacherSample window
  // collect data for 2000 ms or two seconds
  while (millis() - startMillis < sampleWindow)
  {
    TeacherSample = analogRead(0); //TeacherSample from the teacher mic
    TeacherVoltage = abs((TeacherSample * 5) / 1024 - 1.65); //convert teachmic info to voltage

    //Instantaneous Sound pressure level of teacher aka SNR for teacher
    TeacherISPL = 20 * log10(TeacherVoltage / ReferenceSoundPower);

    //while the ISPL of the teacher is below a certain level, start taking samples from the student mic

    //keeps track of the number of samples taken in the time that the teacher was not talking
    Samples = 0;
    while (TeacherISPL < 30)
    {
      StudentSample = analogRead(1); //TeacherSample from the student mic
      StudentVoltage = abs((StudentSample * 5) / 1024 - 1.65); //convert stumic info to 
      StudentISPL[Samples] = 20 * log10(StudentVoltage / ReferenceSoundPower); //Instantaneous Sound pressure level of students aka SNR for student

      //Check if the teacher is talking or not
      TeacherSample = analogRead(0);
      TeacherVoltage = abs((TeacherSample * 5) / 1024 - 1.65);
      TeacherISPL = 20 * log10(TeacherVoltage / ReferenceSoundPower);

      Samples++;
    }
    for (int i = 0; i < Samples; i++)
    {
      LeqSumStudent += pow(10, StudentISPL[i] / 10);
    }
    //Basically and average over the number of samples
    StudentLeq = 10 * log10(1 / Samples * LeqSumStudent); //Equivalent level at the end of the sample window



    //put the samples in an array 
    //once the ISPL of teacher is above threshold, calculate the LEQ of the 
    //Add the number to some variable for averaging 
    //increase the counter so that you know to divide by

    //StudentSample = analogRead(1); //TeacherSample from the student mic
    //StudentVoltage = abs((StudentSample * 5) / 1024 - 1.65); //convert stumic info to voltage

    //  //Should we be using this since it gives the RMS over a period of time vs the ISPL?
    //    StudentSummation = StudentSummation + StudentNoise[SampleNumber] * StudentNoise[SampleNumber];//calculating the summation for RMS of Student
    //    TeacherSummation = TeacherSummation + TeacherNoise[SampleNumber] * TeacherNoise[SampleNumber]; //calculating the summation for RMS of Teacher
    //  
    ////Either we use this or RMS voltage
    //  TeacherISPL[SampleNumber] = 20 * log10(TeacherNoise[SampleNumber] / ReferenceSoundPower); //Instantaneous Sound pressure level of teacher aka SNR for teacher
    //  StudentISPL[SampleNumber] = 20 * log10(StudentNoise[SampleNumber] / ReferenceSoundPower); //Instantaneous Sound pressure level of students aka SNR for student
    //  
    //  
    //  //make and array that stores the Li instantanous sound pressure level. pi is the rms of the voltage signal p0 is the constant
    //  //Find the equivalent level at the moment of done comparing for i = 1 to n
    //  //this will take the leq at the current time based on the array of ISPL
    //  for (int i = 0; i < SampleNumber; i++)
    //  {
    //    LeqSumStudent += pow(10,StudentISPL[i]/10);
    //    LeqSumTeacher += pow(10,TeacherISPL[i]/10);
    //  }
    //  
    //  //Basically and average over the number of samples
    //  TeacherLeq = 10 * log10(1/SampleNumber * LeqSumTeacher); //Equivalent level at the end of 20 ms
    //  StudentLeq = 10 * log10(1/SampleNumber * LeqSumStudent); //Equivalent level at the end of 20 ms
    //  

  }

  //do the comparison of leq
  //if greater than threshold than we start over
  //if not greater, then we do processing until it it over
  //Arbitrary number of 10
  //not sure if this is right. Will need to resolve <--
  float LeqDifference = TeacherLeq - StudentLeq;
  if (LeqDifference > 30)
  {
    //Nothing is to be done and the loop continues
  }
  else
  {
    //LED activation
    if (LeqDifference <= 30)
    {
      digitalWrite(11, HIGH);
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);
    }
    else if ((LeqDifference > 38) && (LeqDifference <= 45))
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
  }

}


