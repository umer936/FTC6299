#pragma config(Sensor, S2,     HTSPB,          sensorI2CCustom9V)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "drivers\hitechnic-superpro.h"
/*
												sendArduinoCommand():
Supporting method for getMPUHeading() and getMPUAccel()
Command numbers correlate with different functions compiled to the Arduino

Command 1: Tell the arduino to blink LED (debug)
Command 6: Reset the gyro for callibration

Command 2: Receive the first half of heading from the arduino
Command 3: Receive the second half of heading from the arduino
NOTE: To receive the full heading, call getMPUHeading(), which
bitwise or pairs the values to return actual heading.


Command 4: receive raw acceleration on the x axis
(Command 5: receive the second half of raw acceleration from the arduino)
NOTE: To receive full acceleration, call getMPUAccel(), which bitwise
or pairs the two values to return the actual heading.
*/

signed int sendArduinoCommand(unsigned char command)
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
short getMPUHeading()
{
	signed char add1 = sendArduinoCommand(2);
	signed char add2 = sendArduinoCommand(3);
	short MPUheading = add1 | (add2 << 8);
	return MPUheading;
}

short getMPUAccelX()
{
	signed char raw = sendArduinoCommand(4);
	nxtDisplayBigTextLine(6, "%d", raw);
	signed char raw2 = sendArduinoCommand(5);
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
