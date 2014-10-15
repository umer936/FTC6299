#pragma config(Sensor, S1,     HTSPB,          sensorI2CCustom9V)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "hitechnic-superpro.h"
/*
Command numbers correlate with different functions compiled to the Arduino
command 1: tell the arduino to calculate heading (input)
command 2: receive the first half of heading from the arduino (output)
command 3: receive raw acceleration on the x axis

*/

signed int result;//Global in order to read on NXT screen

//Support method that sends the command to Arduino for heading
int sendArduinoCommand(unsigned char command)
{
	//add result as local for libs
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
		MPUheading = heading - 360;
	return MPUheading;
}

int getMPUAccelX()
{
	int raw = sendArduinoComman(3);
	return raw;
}

task main()
{
	while(true){
		getMPUHeading();
		nxtDisplayBigTextLine(1, "%d", getMPUHeading());
	}
}
