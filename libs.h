#include "drivers\hitechnic-gyro.h";
#include "drivers\hitechnic-irseeker-v2.h"
#include "drivers\hitechnic-superpro.h"

float heading = 0;

//Support method that sends the command to Arduino for heading
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
short getMPUHeading() //int
{
	//all originally ints
	signed char add1 = sendArduinoCommand(2);
	signed char add2 = sendArduinoCommand(5);
	short MPUheading = add1 | (add2 << 8); //* 2;
	/*if(MPUheading > 180)
		MPUheading = MPUheading - 360;*/
	return MPUheading;
}

short getMPUAccelX()
{
	signed char raw = sendArduinoCommand(3);
	nxtDisplayBigTextLine(6, "%d", raw);
	signed char raw2 = sendArduinoCommand(4);
	return raw | (raw2 << 8); // originally <<*/
}

float valInRange(float val, float threshold = 1.0) {
  return (abs(val) <= threshold) ? 0 : val;
}

bool isInRange(float heading, float targetHeading, float threshold = 1.0) {
  return abs(heading - targetHeading) <= threshold;
}

int getEncoderAverage(int leftMotor, int rightMotor) {
  /*if (abs(leftMotor) < 3) {
    return rightMotor;
  }
  if (abs(rightMotor) < 3) {
    return leftMotor;
  }*/
  return (leftMotor + rightMotor) / 2;
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

void moveTo(int power, int deg, float threshold = 2.0, long time = 5000, float cor = 4.0) {
  heading = 0;
  nMotorEncoder[motorFL] = 0;
  nMotorEncoder[motorFR] = 0;

  wait1Msec(500);
  //HTGYROstartCal(SENSOR_GYRO);
  wait1Msec(500);

  clearTimer(T1);

  if (power > 0) {
    while (time1[T1] < time && getEncoderAverage(nMotorEncoder[motorFL], nMotorEncoder[motorBL]) < deg) {
      displayCenteredBigTextLine(3, "%2i", nMotorEncoder[motorBL]);
      // Reads gyros rate of turn, mulitplies it by the time passed (20ms), and adds it to the current heading
      heading = getMPUHeading();

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
    while (time1[T1] < time && getEncoderAverage(nMotorEncoder[motorFL], nMotorEncoder[motorFR]) > deg) {
      // Reads gyros rate of turn, mulitplies it by the time passed (20ms), and adds it to the current heading
      heading = getMPUHeading();

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

void turn(int power, int deg, int time = 5000) {

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

  wait1Msec(500);
  //HTGYROstartCal(SENSOR_GYRO);
  wait1Msec(500);

  clearTimer(T1);

  if (deg > 0) {
    while (time1[T1] < time && abs(heading) < abs(deg)) {
      heading = getMPUHeading();
      setMotors(power, -power);
      wait1Msec(20);
    }
  }

  if (deg < 0) {
    while (time1[T1] < time && abs(heading) < abs(deg)) {
    heading = getMPUHeading();
      setMotors(-power, power);
      wait1Msec(20);
    }
  }

  stopMotors();
}

void arcTurn(int power, int deg, int time = 2000) {

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
  clearTimer(T1);
  //HTGYROstartCal(SENSOR_GYRO);

  // Forward arcTurn
  if (power > 0) {
    if (deg > 0) {
      while (time1[T1] < time && abs(heading) < abs(deg)) {
        heading = getMPUHeading();
        setMotors(power, 0);
        wait1Msec(20);
      }
    }

    else {
      while (time1[T1] < time && abs(heading) < abs(deg)) {
        heading = getMPUHeading();
        setMotors(0, power);
        wait1Msec(20);
      }
    }
  }

  // Backward arcTurn (flips inequalities)
  else {
    if (deg > 0) {
      while (time1[T1] < time && abs(heading) < abs(deg)) {
        heading = getMPUHeading();
        setMotors(power, 0);
        wait1Msec(20);
      }
    }

    else {
      while (time1[T1] < time && abs(heading) < abs(deg)) {
        heading = getMPUHeading();
        setMotors(0, power);
        wait1Msec(20);
      }
    }
  }
  stopMotors();
}

void latch(bool position) {
  if (!position) {
    servo[servoL] = 225;
    servo[servoR] = 0;
  }
  if (position) {
    servo[servoL] = 150;
    servo[servoR] = 93;
  }
}

int getIR(){
  return HTIRS2readACDir(SENSOR_IR);
}

void moveIrUp(int speed, int IR,){
  while (getIR() < IR) {
    setMotors(speed,speed);
  }
  stopMotors();
}

void moveIrDown(int speed, int IR){
  while (getIR() > IR) {
    setMotors(speed,speed);
  }
  stopMotors();
}
