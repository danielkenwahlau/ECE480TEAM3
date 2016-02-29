//Classroom Noise Monitor 2.0

//constants
const int sampleWindow = 20; // Sample window width in mS (50 mS = 20Hz)
const int ArraySize = 80;
const float ReferenceSoundPower = 0.000002; //2E-6
const int ThresholdA = 30; //The threshold between green and yellow
const int ThresholdB = 23; //The threshold between yellow and red
const float TimeStep = 1/sampleWindow; //20ms
const float DT = TimeStep/sampleWindow; //Timestep/number of samples
//variables
float TeacherSample; //Instantaneous input from teacher mic
float StudentSample; //Instantaneous input from student mic
unsigned int counter;
unsigned int counter2;
float TeacherRms = 0;
float StudentRms = 0;
float TeacherAvgVoltArray[24];
float StudentAvgVolt;
float Snr;
float TeacherISPL[ArraySize];
float StudentISPL[ArraySize];
int counter3 = 0;
float TeacherAvgSum = 0;
float LeqDifference = 0;


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

  
  float TeacherSampleVoltage[ArraySize];
  float StudentReadingVoltage[ArraySize];
  float TeacherLeq;
  float StudentLeq;
  int counter = 0;
  float LeqSumStudent = 0;
  float LeqSumTeacher = 0;
  unsigned long startMillis = millis(); // Start of TeacherSample window
  // collect data for 20 ms
  while (millis() - startMillis < sampleWindow)
  {
      TeacherSample = analogRead(0); //Sample from the teacher mic
      StudentSample = analogRead(1); //Sample from the student mic
    TeacherSampleVoltage[counter] = abs((TeacherSample * 5) / 1024 - 1.65); //convert teachmic info to voltage
      StudentReadingVoltage[counter] = abs((StudentSample * 5) / 1024 - 1.65); //convert stumic info to voltage
    
  //need some way to calculate rms voltage for input into ISPL
  
    TeacherISPL[counter] = 20 * log10(TeacherSampleVoltage[counter] / ReferenceSoundPower); //Instantaneous Sound pressure level of teacher
    StudentISPL[counter] = 20 * log10(StudentReadingVoltage[counter] / ReferenceSoundPower); //Instantaneous Sound pressure level of students
      counter++;
  }

  //calculate the Leq at the end of getting the ISPL array
    for (int i = 0; i < counter; i++)
    {
      LeqSumStudent += pow(10,StudentISPL[i]/10);
      LeqSumTeacher += pow(10,TeacherISPL[i]/10);
    }
    //not sure if this is right. Will need to resolve <--
    TeacherLeq = 10 * log10(TimeStep/DT * LeqSumTeacher); //Equivalent level at the end of 20 ms
    StudentLeq = 10 * log10(TimeStep/DT * LeqSumStudent); //Equivalent level at the end of 20 ms

  
  //do the comparison of leq
  //if greater than threshold than we start over
  //if not greater, then we do processing until it it over
  //Arbitrary number of 10
  //not sure if this is right. Will need to resolve <--
  LeqDifference = TeacherLeq - StudentLeq;
  
  if (LeqDifference >= ThresholdA) //means that the teacher is talking
  {
    //Nothing is to be done and the loop continues since the teacher is talking. 
  digitalWrite(11, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
  }   
  //LED activation
  //Between these two thresholds is the yellow zone
  else if ((LeqDifference < ThresholdA) && (LeqDifference >= ThresholdB))
  {
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
  }
  //This anthing lower than ThresholdB will be considered red
  else
  {
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);
  }   


}

