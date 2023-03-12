#include <iostream>
#include "gps.h"
#include <string>

std::string inputBuffer = "$GPGGA,181908.00,3404.7041778,N,07044.3966270,W,4,13,1.00,495.144,M,29.200,M,0.10,0000*40";
std::string outputBuffer;

GPS myGPS;

int main()
{
	myGPS.read_Data(inputBuffer);
	myGPS.edit_Data();
	myGPS.send_Data(outputBuffer);
	return 0;
}
