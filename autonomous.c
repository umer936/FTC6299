#pragma config(Hubs,  S1, HTMotor,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     HTSPB,          sensorI2CCustom9V)
#pragma config(Sensor, S4,     SENSOR_IR,      sensorI2CCustom)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     motorL,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     motorR,        tmotorTetrix, openLoop, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "libs.h";
#include "autonomous_chooser.h";
//#include "autonomous_methods.h";
#include "drivers\JoystickDriver.c";

/*task ReadRot()
{
	while(true)
	{
		ROT = getMPUrot();
	}
}*/

task main(){
	//startTask(ReadRot);
	moveTo(20, 10000);
	while(true){}
}
