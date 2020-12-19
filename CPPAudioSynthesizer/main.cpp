#include<iostream>
using namespace std;

#include "olcNoiseMaker.h"

//create class that uses frequency formula with given time to generate sound
double MakeSound(double dTime)
{
	//frequeny and hertz conversion to angular velocity (440.0 hertz == A4)
	return 0.5 * sin(880.0 * 2 * 3.14159 * dTime);
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

	while (1)
	{

	}
	return 0;

};