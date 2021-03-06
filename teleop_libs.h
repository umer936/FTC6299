#include "drivers\hitechnic-gyro.h";
#include "drivers\hitechnic-irseeker-v2.h"

/*
	Made by Team 6299 QuadX
		- Jacob Greenway
		- Joshua Johnson
		- Linnea May
*/
float heading = 0;

float valInRange(float val, float threshold = 1.0) {
	return (abs(val) <= threshold) ? 0 : val;
}

bool isInRange(float heading, float targetHeading, float threshold = 1.0) {
	return abs(heading - targetHeading) <= threshold;
}

int getEncoderAverage() {
	int divide = 4;
	if (nMotorEncoder[motorFL] == 0) {
		divide--;
	}
	if (nMotorEncoder[motorBL] == 0) {
		divide--;
	}
	if (nMotorEncoder[motorFR] == 0) {
		divide--;
	}
	if (nMotorEncoder[motorBR] == 0) {
		divide--;
	}
	return (nMotorEncoder[motorFL] + nMotorEncoder[motorBL] + nMotorEncoder[motorFR] + nMotorEncoder[motorBR]) / divide;
}

void setMotors(int left, int right) {
	motor[motorFL] = left;
	motor[motorBL] = left;
	motor[motorFR] = right;
	motor[motorBR] = right;
}

void stopMotors() {
	motor[motorFL] = 0;
	motor[motorBL] = 0;
	motor[motorFR] = 0;
	motor[motorBR] = 0;
}

void moveTo(int power, int deg, float threshold = 2.0, long time = 100000, float cor = 4.0) {
	heading = 0;
	nMotorEncoder[motorFL] = 0;
	nMotorEncoder[motorFR] = 0;
	wait1Msec(250);
	clearTimer(T1);
	if (power > 0) {
		while (time1[T1] < time && getEncoderAverage() < deg) {
			// Reads gyros rate of turn, mulitplies it by the time passed (20ms), and adds it to the current heading
			heading += valInRange(HTGYROreadRot(SENSOR_GYRO), threshold) * (float)(20 / 1000.0);

			// Checks if the gyro is outside of the specified threshold (1.0)
			if (isInRange(heading, 0, threshold)) {
				setMotors(power, power);
			}

			// If not, lower the speed of the required side of the robot to adjust back to 0
			else {
				if (heading > 0) {
					setMotors((power / cor), power);
				}
				if (heading < 0) {
					setMotors(power, (power / cor));
				}
			}
			wait1Msec(20);
		}
	}

	else {
		while (time1[T1] < time && getEncoderAverage() > deg) {
			// Reads gyros rate of turn, mulitplies it by the time passed (20ms), and adds it to the current heading
			heading += valInRange(HTGYROreadRot(SENSOR_GYRO), threshold) * (float)(20 / 1000.0);

			// Checks if the gyro is outside of the specified threshold (1.0)
			if (isInRange(heading, 0, threshold)) {
				setMotors(power, power);
			}

			// If not, lower the speed of the required side of the robot to adjust back to 0
			else {
				if (heading > 0) {
					setMotors(power, (power / cor));
				}
				if (heading < 0) {
					setMotors((power / cor), power);
				}
			}

			wait1Msec(20);
		}
	}

	stopMotors();
}

void turn(int power, int deg, int time = 6000) {

	heading = 0;
	wait1Msec(250);
	clearTimer(T1);

	if (deg > 0) {
		while (time1[T1] < time && abs(heading) < abs(deg)) {
			heading += HTGYROreadRot(SENSOR_GYRO) * (float)(20 / 1000.0);
			setMotors(power, -power);
			wait1Msec(20);
		}
	}

	if (deg < 0) {
		while (time1[T1] < time && abs(heading) < abs(deg)) {
		heading += HTGYROreadRot(SENSOR_GYRO) * (float)(20 / 1000.0);
			setMotors(-power, power);
			wait1Msec(20);
		}
	}

	stopMotors();
}

void arcTurn(int power, int deg, int time = 7000) {

	// 90 Degree Modifier
	if (abs(deg) == 90) {
		int modifier = deg * 8/9;
		deg = modifier;
	}

	// 45 Degree Modifier
	else if (abs(deg) == 45) {
		int modifier = deg * 7/9;
		deg = modifier;
	}

	heading = 0;
	wait1Msec(250);
	clearTimer(T1);
	// Forward arcTurn
	if (power > 0) {
		if (deg > 0) {
			while (time1[T1] < time && abs(heading) < abs(deg)) {
				heading += HTGYROreadRot(SENSOR_GYRO) * (float)(20 / 1000.0);
				setMotors(power, 0);
				wait1Msec(20);
			}
		}

		else {
			while (time1[T1] < time && abs(heading) < abs(deg)) {
				heading += HTGYROreadRot(SENSOR_GYRO) * (float)(20 / 1000.0);
				setMotors(0, power);
				wait1Msec(20);
			}
		}
	}

	// Backward arcTurn (flips inequalities)
	else {
		if (deg > 0) {
			while (time1[T1] < time && abs(heading) < abs(deg)) {
				heading += HTGYROreadRot(SENSOR_GYRO) * (float)(20 / 1000.0);
				setMotors(power, 0);
				wait1Msec(20);
			}
		}

		else {
			while (time1[T1] < time && abs(heading) < abs(deg)) {
				heading += HTGYROreadRot(SENSOR_GYRO) * (float)(20 / 1000.0);
				setMotors(0, power);
				wait1Msec(20);
			}
		}
	}
	stopMotors();
}

void drift(int power, int deg, int angle, int time = 8000) {
	heading = 0;
	nMotorEncoder[motorFL] = 0;
	nMotorEncoder[motorFR] = 0;
	wait1Msec(250);
	clearTimer(T1);
	if (power > 0) {
		while (time1[T1] < time && getEncoderAverage() < deg) {
			heading += HTGYROreadRot(SENSOR_GYRO) * (float)(20 / 1000.0);
			if (angle > 0) {
				while (heading < angle) {
					setMotors(power, power - (power / 10));
				}
			}
			else {
				while (heading > angle) {
					setMotors(power - (power / 10), power);
				}
			}
		}
	}
	if (power < 0) {
		while (time1[T1] < time && getEncoderAverage() < deg) {
			heading += HTGYROreadRot(SENSOR_GYRO) * (float)(20 / 1000.0);
			if (angle > 0) {
				while (heading < angle) {
					setMotors(power, power - 10);
				}
			}
			else {
				while (heading > angle) {
					setMotors(power - 10, power);
				}
			}
		}
	}
	stopMotors();
}

void latch(bool position) {
	wait1Msec(250);
	if (!position) {
		servo[servoL] = 225;
		servo[servoR] = 20;
	}
	if (position) {
		servo[servoL] = 150;
		servo[servoR] = 93;
	}
	wait1Msec(250);
}

int getPos() {
	wait1Msec(175);
	if(HTIRS2readACDir(SENSOR_IR) == 0)
		return 1;
	if(HTIRS2readACDir(SENSOR_IR) == 4)
		return 2;
	if(HTIRS2readACDir(SENSOR_IR) == 5)
		return 3;
	return 0;
}

void lift(int power, int time) {
	int t = 0;
	wait1Msec(250);
	while (t < time) {
		motor[liftL] = power;
		motor[liftR] = power;
		t += 20;
		wait1Msec(20);
	}
	motor[liftL] = 0;
	motor[liftR] = 0;
}

void basket(char position) {

	if (position == 'x') {
		servo[liftServoL] = 100;
		servo[liftServoR] = 155;
	}
	if (position == 'y') {
		servo[liftServoL] = 230;
		servo[liftServoR] = 25;
	}
}
