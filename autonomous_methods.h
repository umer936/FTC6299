#include "libs.h";

void initializeRobot() {
	nVolume = 4;
	latch(false);
	basket('y');
}

// Finds the ir beacon, dumps the block, and parks on the ramp
void bare(int suspend, int begin) {
	delay(suspend * 100);
	if (begin == 1) {  //Parking Zone
		playSound(soundBeepBeep);
		lift(40, 3250);
		basket('x');
	}
	if (begin == 0) {  //Ramp
		playSound(soundBeepBeep);
		moveTo(-50, -3900, 2.1);
		latch(true);
		arcTurn(17, 30);
		moveTo(15, 700, 0.8, 2000);
		turn(30, 90);
		turn(30, 90);
		moveTo(-25, -3000, 2.0);
		lift(40, 3250);
		basket('x');
		latch(false);
	}
}

void autoFrontDump(int suspend) {
	delay(suspend * 100);

}

void test(int suspend){
	delay(suspend * 100);

}
