#pragma config(Sensor, S1,     HTSPB,          sensorI2CCustom9V)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "hitechnic-superpro.h"
/*
Command numbers correlate with different functions compiled to the Arduino
command 1: Tell the arduino to blink LED (debug)
command 2: receive the first half of heading from the arduino (output)
command 3: receive raw acceleration on the x axis
command 4: receive the second half of raw acceleration from the arduino

*/

//Support method that sends the command to Arduino for heading
int sendArduinoCommand(unsigned char command)
{
	signed int result;
	HTSPBsetupIO(HTSPB, 0xFF); //sets B0-7 to output
	HTSPBwriteStrobe(HTSPB, command); // send the command via S0-3
	if (command >= 2)
	{
		HTSPBsetupIO(HTSPB, 0x00); // sets BO-7 to input so that it can receive
		result = HTSPBreadIO(HTSPB, 0xFF);
	}
	return result;
}

//Get the current heading from the MPU6050 gyro
int getMPUHeading()
{
	int add1 = sendArduinoCommand(2);
	int MPUheading = add1 * 2;
	if(MPUheading > 180)
		MPUheading = MPUheading - 360;
	return MPUheading;
}

int getMPUAccelX()
{
	short raw = sendArduinoCommand(3);
	short raw2 = sendArduinoCommand(4);
	return raw | (raw2 << 8);
}

task main()
{
	while(true){
		getMPUHeading();
		nxtDisplayBigTextLine(1, "%d", getMPUHeading());
		getMPUAccelX();
		nxtDisplayBigTextLine(4, "%d", getMPUAccelX());

	}
}
