//Classroom Noise Monitor 2.0


int counterHistory;
int historyArray[25];
int historySum;
const int sampleWindow = 20; // Sample window width in mS (50 mS = 20Hz)
const int ArraySize = 80;
float TeacherSample;
float StudentSample;
unsigned int counter;
unsigned int counter2;
float TeacherMean;
float StudentMean;
float TeacherSize;
float StudentSize;
float TeacherRms = 0;
float StudentRms = 0;
float TeacherAvgVoltArray[24];
float StudentAvgVolt;
float Snr;
float TeacherISPL[ArraySize];
float StudentISPL[ArraySize];
float ReferenceSoundPower = 0.000002; //2E-6
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
  float TeacherNoise[ArraySize];
  float StudentNoise[ArraySize];
  float TeacherLeq;
  float StudentLeq;
  int counter = 0;
  int counter2 = 0;
  float summation = 0;
  float summation2 = 0;
  float LeqSumStudent = 0;
  float LeqSumTeacher = 0;
  unsigned long startMillis = millis(); // Start of TeacherSample window
  // collect data for 20 ms
  while (millis() - startMillis < sampleWindow)
  {
    if (counter < StudentSize) // toss out spurious readings
    {
      TeacherSample = analogRead(0); //TeacherSample from the teacher mic
      if (abs(TeacherSample * 5 / 1024) > .001)//ignore silent samples
      {
        TeacherNoise[counter2] = abs((TeacherSample * 5) / 1024 - 1.65); //convert teachmic info to voltage
        counter2++;
      }
      StudentSample = analogRead(1); //TeacherSample from the student mic
      StudentNoise[counter] = abs((StudentSample * 5) / 1024 - 1.65); //convert stumic info to voltage
      summation2 = summation2 + StudentNoise[counter] * TeacherNoise[counter];//calculate student rms
      summation = summation + TeacherNoise[counter] * TeacherNoise[counter]; //calculate teacher rms
    
    TeacherISPL[counter] = 20 * log10(TeacherNoise[counter] / ReferenceSoundPower); //Instantaneous Sound pressure level of teacher
    StudentISPL[counter] = 20 * log10(StudentNoise[counter] / ReferenceSoundPower); //Instantaneous Sound pressure level of students
    
    
    //make and array that stores the Li instantanous sound pressure level. pi is the rms of the voltage signal p0 is the constant
    //Find the equivalent level at the moment of done comparing for i = 1 to n
    //
    //this will take the leq at the current time based on the array of ISPL
    for (int i = 0; i < counter; i++)
    {
      LeqSumStudent += pow(10,StudentISPL[i]/10);
      LeqSumTeacher += pow(10,TeacherISPL[i]/10);
    }
    float TimeStep = .02;
    float DT = .02/20;
    TeacherLeq = 10 * log10(TimeStep/DT * LeqSumTeacher); //Equivalent level at the end of 20 ms
    StudentLeq = 10 * log10(TimeStep/DT * LeqSumStudent); //Equivalent level at the end of 20 ms
    
      counter++;
    }
  }
  
  //do the comparison of leq
  //if greater than threshold than we start over
  //if not greater, then we do processing until it it over
  
  //Arbitrary number of 10
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

