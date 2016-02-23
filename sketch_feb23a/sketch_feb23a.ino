//adding test name here
//Testing 2

int counterHistory;
int historyArray[25];
int historySum;
const int sampleWindow = 20; // Sample window width in mS (50 mS = 20Hz)
float sample;
float sample2;
unsigned int counter;
unsigned int counter2;
float tmean;
float smean;
float tsize;
float ssize;
float trms = 0;
float srms = 0;
float teachavgvoltarray[24];
float studentavgvolt;
float snr;
int counter3 = 0;
float tavgsum = 0;
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
  float tnoise[80];
  float snoise[80];
  int counter = 0;
  int counter2 = 0;
  float summation = 0;
  float summation2 = 0;
  unsigned long startMillis = millis(); // Start of sample window
  // collect data for 20 ms
  while (millis() - startMillis < sampleWindow)
  {
    if (counter < ssize) // toss out spurious readings
    {
      sample = analogRead(0); //sample from the teacher mic
      if (abs(sample * 5 / 1024) > .001)//ignore silent samples
      {
        tnoise[counter2] = abs((sample * 5) / 1024 - 1.65); //convert teachmic info to voltage
        counter2++;
      }
      sample2 = analogRead(1); //sample from the student mic
      snoise[counter] = abs((sample2 * 5) / 1024 - 1.65); //convert stumic info to voltage
      summation2 = summation2 + snoise[counter] * tnoise[counter];//calculate student rms
      summation = summation + tnoise[counter] * tnoise[counter]; //calculate teacher rms
      counter++;
    }
  }
  int n = 0;
  tsize = counter2;
  ssize = sizeof(snoise) / sizeof(float);
  trms = summation / tsize;
  srms = summation2 / ssize;
  snr = 20 * log10(trms / srms);
  //history buffer
  if (snr > 6)
  {
    historyArray[counterHistory] = 0;
  }
  else if ((snr <= 6) && (snr >= -2))
  {
    historyArray[counterHistory] = 1;
  }
  else if (20 * log10(studentavgvolt / .00002)>95)
  {
    historyArray[counterHistory] = 2;
  }
  else
  {
    historyArray[counterHistory] = 2;
  }
  //increment history counter
  counterHistory++;
  //reset history counter to front
  if (counterHistory > 24)
  {
    counterHistory = 0;
  }
  historySum = 0;
  for (int j = 0; j < 25; j++)
  {
    historySum += historyArray[j];
  }
  //LED activation
  if (historySum <= 30)
  {
    digitalWrite(11, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
  }
  else if ((historySum > 38) && (historySum <= 45))
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

