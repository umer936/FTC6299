#pragma config(Hubs,  S3, HTServo,  none,     none,     none)
#pragma config(Hubs,  S4, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S1,     SENSOR_IR,      sensorI2CCustom)
#pragma config(Sensor, S2,     SENSOR_GYRO,    sensorI2CHiTechnicGyro)
#pragma config(Sensor, S3,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S4_C1_1,     motorFL,       tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S4_C1_2,     motorFR,       tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S4_C2_1,     motorBL,       tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S4_C2_2,     motorBR,       tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S4_C3_1,     liftL,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S4_C3_2,     liftR,         tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S4_C4_1,     flip,          tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S4_C4_2,     motorI,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S3_C1_1,    servoL,               tServoStandard)
#pragma config(Servo,  srvo_S3_C1_2,    servoR,               tServoStandard)
#pragma config(Servo,  srvo_S3_C1_3,    liftServoL,           tServoStandard)
#pragma config(Servo,  srvo_S3_C1_4,    liftServoR,           tServoStandard)
#pragma config(Servo,  srvo_S3_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c";
#include "libs.h";
task flipperFlapper() {
	//displayTextLine(0, "%7i", -12 % 10);
	//nMotorEncoder[flip] = 0;
	bool flipin = false;
	while (true) {
		while (joy1Btn(07) == 1) {
			motor[flip] = 100;
		}
		while (joy1Btn(08) == 1) {
			motor[flip] = -100;
		}
		delay(50);
		if (!flipin && joy1Btn(03) == 1) {
			flipin = true;
			delay(100);
		}
		while (flipin) {
			motor[flip] = -100;
			if (joy1Btn(03) == 1 || joy1Btn(07) == 1 || joy1Btn(08) == 1) {
				flipin = false;
				delay(100);
			}
		}
		motor[flip] = 0;
		/*if (joystick.joy1_TopHat == 4) {
			int move = nMotorEncoder[flip] % 570;
			move = 570 - move;
			nMotorEncoder[flip] = 0;
			while (abs(nMotorEncoder[flip]) < move) {
				motor[flip] = -35;
				displayTextLine(0, "%7i", nMotorEncoder[flip]);
				wait1Msec(5);
			}
		}*/
		//displayTextLine(0, "%7i", nMotorEncoder[flip]);
	}
}
task driveControl() {
	bool slowmo = false;
	while (true) {
		getJoystickSettings(joystick);
		if ((abs(joystick.joy1_y1) > 10 || abs(joystick.joy1_y2) > 10) && !slowmo) {
			motor[motorFL] = joystick.joy1_y1 / 1.28;
			motor[motorBL] = joystick.joy1_y1 / 1.28;
			motor[motorFR] = joystick.joy1_y2 / 1.28;
			motor[motorBR] = joystick.joy1_y2 / 1.28;
		}
		else if ((abs(joystick.joy1_y1) > 10 || abs(joystick.joy1_y2) > 10) && slowmo) {
			motor[motorFL] = joystick.joy1_y1 / 5.28;
			motor[motorBL] = joystick.joy1_y1 / 5.28;
			motor[motorFR] = joystick.joy1_y2 / 5.28;
			motor[motorBR] = joystick.joy1_y2 / 5.28;
		}
		else {
			motor[motorFL] = 0;
			motor[motorBL] = 0;
			motor[motorFR] = 0;
			motor[motorBR] = 0;
			while (joy1Btn(02) == 1) {
				float threshold = 0.8;
				int power = 80;
				displayCenteredBigTextLine(3, "%2i", heading);
				heading += valInRange(HTGYROreadRot(SENSOR_GYRO), threshold) * (float)(20 / 1000.0);

				// Checks if the gyro is outside of the specified threshold (1.0)
				if (isInRange(heading, 0, threshold)) {
					setMotors(power, power);
				}

				// If not, lower the speed of the required side of the robot to adjust back to 0
				else {
					if (heading > 0) {
						setMotors((power / 4), power);
					}
					if (heading < 0) {
						setMotors(power, (power / 4));
					}
				}
				wait1Msec(20);
			}
		}
		if(!slowmo && joystick.joy1_TopHat == 2) {
			slowmo = true;
			wait1Msec(300);
		}
		if(slowmo && joystick.joy1_TopHat == 2) {
			slowmo = false;
			wait1Msec(300);
		}
	}
}

task servos() {
	while (true) {
		if (joy1Btn(05) == 1) {
			servo[servoL] = 250;
			servo[servoR] = 0;
		}
		if (joy1Btn(06) == 1) {
			servo[servoL] = 160;
			servo[servoR] = 90;
		}

		// BTN01 = X
		if (joy2Btn(01) == 1) {
			servo[liftServoL] = 100;
			servo[liftServoR] = 145;
		}

		// BTN04 = Y
		if (joy2Btn(04) == 1) {
			servo[liftServoL] = 240;
			servo[liftServoR] = 05;
		}
		// BTN02 = A
		if (joy2Btn(02) == 1) {
			servo[liftServoL] = 220;
			servo[liftServoR] = 25;
		}
		// BTN03 = B
		if (joy2Btn(03) == 1) {
			servo[liftServoL] = 120;
			servo[liftServoR] = 125;
		}
	}
}

task slide()
{
	while(true){
		while(abs(joystick.joy2_y1) >= 10){
			motor[liftL] = joystick.joy2_y1 / -1.28;
			motor[liftR] = joystick.joy2_y1 / -1.28;
		}
		motor[liftL] = 0;
		motor[liftR] = 0;
	}
}
task main () {
	waitForStart();
	wait1Msec(50);
	HTGYROstartCal(SENSOR_GYRO);
	heading = 0;
	wait1Msec(50);
	startTask(driveControl);
	startTask(servos);
	startTask(flipperFlapper);
	startTask(slide);
	while(true){}
}
