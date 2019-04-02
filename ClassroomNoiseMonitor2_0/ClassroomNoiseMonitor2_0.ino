//Classroom Noise Monitor 2.0

const int sampleWindow = 2000;        // Sample window width in mS
const int SampleArraySize = 80;       // Max size of sample array
int TeacherMicThreshold = 10000;	  //Threshold used to compare if the teacher is talking or not (Must be tuned)
float ReferenceSoundPower = 0.000002; //2E-6 This is the reference sound pressure of Air
float TeacherAvgSum = 0;
float ThresholdA = 30;				  //Threshold between green and yellow
float ThresholdB = 38;				  //Threshold between yellow and Red
float constant = 50;				  //For future use this is the number we will have to adjust when live signal testing


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
	float TeacherLeq = 0;
	float StudentLeq = 0;
	float LeqSumStudent = 0;
	float LeqSumTeacher = 0;
	unsigned int NumStudentSamples = 0;        //represents the student sample number that we are on
	unsigned int NumTeacherSamples = 0;        //represents the teacher sample number that we are on
	float TeacherISPLArray[SampleArraySize];
	float StudentISPLArray[SampleArraySize];
	float LeqTotal = 0;
	float LeqAverage = 0;
	float TeacherSample;          // Raw Quantized Sample from teacher mic
	float StudentSample;          // Raw Quantized Sample from student mic
	unsigned int NumWindows = 0; //The number of windows/times that samples were being taken from the students used to average Leq
	unsigned long startMillis = millis(); // Start of TeacherSample window
	// collect data for 2000 ms or two seconds
	while (millis() - startMillis < sampleWindow)
	{
		TeacherSample = analogRead(0); //TeacherSample from the teacher mic
		TeacherVoltage = abs((TeacherSample * 5) / 1024 - 1.65); //convert TeacherSample to voltage. Subtracted DC offset that was added in the analog side.		
		TeacherISPLArray[NumTeacherSamples] = 20 * log10(TeacherVoltage / ReferenceSoundPower) + constant; //Instantaneous Sound pressure level of teacher aka SNR for teacher
		NumTeacherSamples++;
		//while the ISPL of the teacher is below a certain level, start taking samples from the student mic
		NumStudentSamples = 0; //keeps track of the number of samples taken in the time that the teacher was not talking


		//Check if the teacher is still talking or not
		TeacherISPL = 20 * log10(TeacherVoltage / ReferenceSoundPower);
		while (TeacherISPL < TeacherMicThreshold)
		{
			StudentSample = analogRead(1); //TeacherSample from the student mic
			StudentVoltage = abs((StudentSample * 5) / 1024 - 1.65); //convert stumic info to 
			StudentISPLArray[NumStudentSamples] = 20 * log10(StudentVoltage / ReferenceSoundPower) + constant; //Instantaneous Sound pressure level of students aka SNR for student
			NumStudentSamples++;

			//Check if the teacher microhphone is still active
			TeacherSample = analogRead(0);
			TeacherVoltage = abs((TeacherSample * 5) / 1024 - 1.65);
			TeacherISPL = 20 * log10(TeacherVoltage / ReferenceSoundPower);
		}
		NumWindows++;
		//find LEQ of student over the window
		for (int i = 0; i < NumStudentSamples; i++)
		{
			LeqSumStudent += pow(10, StudentISP[Li] / 10);
		}		
		//Basically an average over the number of samples
		for (int i = 0; i < NumTeacherSamples; i++)
		{
			LeqSumTeacher += pow(10,TeacherISPL[i]/10);
		}

		//total Leq over the sample window 
		TeacherLeq = 10 * log10(LeqSumTeacher/ NumTeacherSamples); //Equivalent level at the end of 20 ms
		StudentLeq = 10 * log10(LeqSumStudent / NumStudentSamples); //Total Leq at the end of the sample window

		//Store the result for averaging
		StudentLeqTotal += StudentLeq;
		TeacherLeqTotal += TeacherLeq;
	}

    //do the comparison of leq
    //if greater than threshold than we start over
    //if not greater, then we do processing until it is over
	StudentLeqAverage = StudentLeqTotal / NumWindows;
	TeacherLeqAverage = TeacherLeqTotal / NumWindows;
    float LeqDifference = TeacherLeqAverage - StudentLeqAverage;

	//LED activation
	if (LeqDifference <= 30) //GREEN
	{
		digitalWrite(11, HIGH);
		digitalWrite(12, LOW);
		digitalWrite(13, LOW);
	}
	else if ((LeqDifference > 38) && (LeqDifference <= 45)) //YELLOW
	{
		digitalWrite(11, LOW);
		digitalWrite(12, HIGH);
		digitalWrite(13, LOW);
	}
	else //RED
	{
		digitalWrite(11, LOW);
		digitalWrite(12, LOW);
		digitalWrite(13, HIGH);
	}
}