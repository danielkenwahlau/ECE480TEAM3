//Classroom Noise Monitor 2.0
const int sampleWindow = 2000;        // Sample window width in mS
const int SampleArraySize = 80;       // Max size of sample array
int TeacherMicThreshold = 50;	  	  //Threshold used to compare if the teacher is talking or not 50 db is about the normal speaking voice
float ReferenceSoundPower = 0.000002; //2E-6 This is the reference sound pressure of Air
float ThresholdA = 30;				  //Threshold between green and yellow
float ThresholdB = 38;				  //Threshold between yellow and Red
float constant = 50;				  //For future use this is the number we will have to adjust when live signal testing


void setup()
{
	Serial.begin(9600);
	//initialize output pins for LEDs
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
	float TeacherVoltage = 0; //Converted voltage reading from microphone
	float StudentVoltage = 0; 
	float TeacherLeq = 0; //total sound energy measure over a period of time for microhphone
	float StudentLeq = 0; 
	float LeqSumStudent = 0; //sound pressure level over and interval of time
	float LeqSumTeacher = 0;
	unsigned int NumStudentSamples = 0;        //represents the sample number that we are on
	unsigned int NumTeacherSamples = 0;        
	float TeacherISPL = 0;					   //Instantaneous Sound Pressure Level at one moment in time in dB
	float TeacherISPLArray[SampleArraySize];   //holds the sample values for the window over the period
	float StudentISPLArray[SampleArraySize];
	float StudentLeqTotal = 0; 			  	   //running total for of Leq measurements over the period
	float TeacherLeqTotal = 0;
	float LeqDifference = 0;				   //The difference between average Leqs of the student and teacher over the period
	float StudentLeqAverage = 0;			   //Average Leq measured over the period
	float TeacherLeqAverage = 0;
	float TeacherSample = 0;          		   //Raw Quantized Sample from teacher mic
	float StudentSample = 0;          		   //Raw Quantized Sample from student mic
	unsigned int NumWindows = 0; 			   //The number of windows/times that samples were being taken from the students used to average Leq
	unsigned long startMillis = millis();      //Start of TeacherSample window
	// collect data for 2000ms (2 seconds)
	while (millis() - startMillis < sampleWindow)
	{
		TeacherSample = analogRead(0); //Digital sample from the teacher mic
		TeacherVoltage = abs((TeacherSample * 5) / 1024 - 2.5); //convert TeacherSample to voltage. Subtracted DC offset that was added in on the analog side.
		TeacherISPL = 20 * log10(TeacherVoltage / ReferenceSoundPower) + constant; //Instantaneous Sound pressure level of teacher
		TeacherISPLArray[NumTeacherSamples] = TeacherISPL; 
		NumTeacherSamples++;
		NumStudentSamples = 0;
		//while the ISPL of the teacher is below a certain level, start taking samples from the student mic
		//Loop exits when the teacher microphone is no longer active
		while (TeacherISPL < TeacherMicThreshold)
		{
			StudentSample = analogRead(1); //Sample from the student mic
			StudentVoltage = abs((StudentSample * 5) / 1024 - 2.5); //convert stumic info to voltage
			StudentISPLArray[NumStudentSamples] = 20 * log10(StudentVoltage / ReferenceSoundPower) + constant; //Instantaneous Sound pressure level of students aka SNR for student
			NumStudentSamples++;

			//Take a new sample to check if the teacher micrphone is still active
			TeacherSample = analogRead(0);
			TeacherVoltage = abs((TeacherSample * 5) / 1024 - 2.5);
			TeacherISPL = 20 * log10(TeacherVoltage / ReferenceSoundPower) + constant;
		}
		//find Leq of over the window
		//If we sampled from the student mic then take the Leq of the sampling window from both the teacher and student
		if (NumStudentSamples > 0 )
		{
			NumWindows++;
			for (int i = 0; i < NumStudentSamples; i++)
			{
				LeqSumStudent += pow(10, StudentISPLArray[Li] / 10);
			}		
			for (int i = 0; i < NumTeacherSamples; i++)
			{
				LeqSumTeacher += pow(10, TeacherISPLArray[i] / 10);
			}
			NumStudentSamples = 0;
			NumTeacherSamples = 0;
			//Calculate the Leq over the sample window for both the teacher and student (anti-log)
			TeacherLeq = 10 * log10(LeqSumTeacher/ NumTeacherSamples);
			StudentLeq = 10 * log10(LeqSumStudent / NumStudentSamples);
			//Store the result that will be used to calcuate and average over the entire two second window
			StudentLeqTotal += StudentLeq;
			TeacherLeqTotal += TeacherLeq;
		}


	}

    //do the comparison of leq
	StudentLeqAverage = StudentLeqTotal / NumWindows;
	TeacherLeqAverage = TeacherLeqTotal / NumWindows;
    float LeqDifference = TeacherLeqAverage - StudentLeqAverage;

	//Activate LED's based on the difference in Leq (aka intelligibility)
	//GREEN
	if (LeqDifference <= ThresholdA) 
	{
		digitalWrite(11, HIGH);
		digitalWrite(12, LOW);
		digitalWrite(13, LOW);
	}
	//YELLOW
	else if ((LeqDifference > ThresholdA) && (LeqDifference <= ThresholdB)) 
	{
		digitalWrite(11, LOW);
		digitalWrite(12, HIGH);
		digitalWrite(13, LOW);
	}
	//RED
	else
	{
		digitalWrite(11, LOW);
		digitalWrite(12, LOW);
		digitalWrite(13, HIGH);
	}
}