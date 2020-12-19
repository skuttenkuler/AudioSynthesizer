
#include <iostream>
using namespace std;

#include "olcNoiseMaker.h"

//SET GLOBAL
//use atomic to synch memory access to key binding thread
atomic<double> dFrequencyOutput = 0.0;
//base octave
double dOctaveBaseFreq = 110.0; //A2
//get standard 12 notes by power of 2 to the 12th root. Divides into 12 notes.
double d12thRootOf2 = pow(2.0, 1.0 / 12.0);

//create class that uses frequency formula with given time to generate sound
// Returns amp (-1.0 to +1.0) as a function of time
double MakeSound(double dTime)
{
	//assign sine wave output to variable to manipulate
	double dOutput = sin(dFrequencyOutput * 2 * 3.14159 * dTime);
	//set master volume
	return dOutput * 0.5;
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


	//sit in the loop, get keyboard state changes and modify output
	//set current key pressed
	int nCurrentKey = -1;
	//set bool for pressed key
	bool bKeyPressed = false;
	//while active
	while (1)
	{
		// add keyboard like on piano
		bKeyPressed = false;

		for (int k = 0; k < 16; k++)
		{
			//map out key to fit piano(C = C, F = C#, X = D, etc)
			if (GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe\xbf"[k])) & 0x8000)
			{
				if (nCurrentKey != k)
				{
				//set frequency
				dFrequencyOutput = dOctaveBaseFreq * pow(d12thRootOf2, k);
				//print out key and Hz
				wcout << "\rNote On : " << sound.GetTime() << "s " << dFrequencyOutput << "Hz";
				nCurrentKey = k;
				}
			bKeyPressed = true;
			}
		}
		//if NOT pressed
		if (!bKeyPressed)
		{
			if (nCurrentKey != -1)
			{
				wcout << "\rNote Off: " << sound.GetTime() << "s                        ";
				nCurrentKey = -1;
			}

			dFrequencyOutput = 0.0;
		}
	}

	return 0;
}

