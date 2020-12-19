#include<iostream>
using namespace std;

#include "olcNoiseMaker.h"

//use atomic to synch memory access to key binding thread
atomic<double> dFrequencyOutput = 0.0;

//create class that uses frequency formula with given time to generate sound
double MakeSound(double dTime)
{
	//assign sine wave output to variable to manipulate
	double dOutput = 1.0 * sin(220.0 * 2 * 3.14159 * dTime);
	//set square wave thresholds
	if (dOutput > 0.0)
		return 0.2;
	else
		return -0.2;
}
int main()
{
	wcout << "Audio Synthesizer" << endl;
	//Get all audio hardware
	vector<wstring> devices = olcNoiseMaker<short>::Enumerate();
	//Display the hardware
	for (auto d : devices) wcout << "Output:" << d << endl;
	//then create instance the first device it finds, the sample rate, num of channels, and latency management with given memory
	olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

	//link MakeSound class with sound maker with function pointer
	sound.SetUserFunction(MakeSound);
	//bass octave
	double dOctaveBassFreq = 110.0; //A2
	//get standard 12 notes by power of 2 to the 12th root. Divides into 12 notes.
	double d12thRootOf2 = pow(2.0, 1.0 / 12.0);

	//while active
	while (1)
	{
		// add keyboard
		//if keypressed specify a frequency
		if (GetAsyncKeyState('A' & 0x8000))
		{
			//set to 440.0 hz
			dFrequencyOutput = dOctaveBassFreq * pow(d12thRootOf2, 0);
		}
		else
		{
			dFrequencyOutput = 0.0;
		}

	}
	return 0;

};