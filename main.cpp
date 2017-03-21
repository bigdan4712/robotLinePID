#include <ev3dev.h>
#include <unistd.h>
#include <iostream>
#include <time.h>

int err = 0;
int i = 0;
int error_array[64] = {0};

void speed_select(int &Kp, float &Ki, float &Kd);
void compute_Ki(int &errorI, int error_array[]);

int main(void) {

	int rightSpeed = 0;
	int leftSpeed = 0;

	int readSensor = 0;
	int errorP = 0;
	int errorI = 0;
	int errorD = 0;
	int lastError = 0;
	const int setPoint = 40;
	int outputSetPoint = 70;
	int offset = 0;
	int Kp = 0;
	float Ki = 0.000;
	float Kd = 0.000;
	int fudge1 =0;
	int fudge2 =0;

	

	ev3Setup();
	ev3MotorReset(MOTOR_A);
	ev3MotorReset(MOTOR_B);
	setInputMode();

	ev3ColorSetMode(SENSOR_1,0);

	speed_select(Kp,Ki,Kd);
	
	ev3MotorSetPower(MOTOR_A,leftSpeed);
	ev3MotorSetPower(MOTOR_B,rightSpeed);

	ev3MotorRun(MOTOR_A);
	ev3MotorRun(MOTOR_B);

	while(!escapeButton.isPressed()) { //code goes here

		readSensor = ev3ColorReadReflected(SENSOR_1);
		errorP = readSensor - setPoint;
		errorI += errorP;
		errorD = readSensor - lastError;

		compute_Ki(errorI, error_array);

		offset = (errorP*Kp)+(errorI*Ki)-(errorD*Kd);

		fudge1 = 100 - outputSetPoint;
		fudge2 = 100 +  outputSetPoint;

		if(offset > 0) {
	
			rightSpeed = outputSetPoint+offset;
			leftSpeed = (outputSetPoint-(offset*fudge2)/fudge1);

		}

		else {
			rightSpeed = outputSetPoint+(offset*fudge2)/fudge1;
			leftSpeed = outputSetPoint-offset;
		}

		

		if(rightSpeed < -100) rightSpeed = -100;
		if(rightSpeed > 100) rightSpeed = 100;
		if(leftSpeed < -100) leftSpeed = -100;
		if(leftSpeed > 100) leftSpeed = 100;


		ev3MotorSetPower(MOTOR_A, -leftSpeed);
		ev3MotorSetPower(MOTOR_B, -rightSpeed);

		lastError = readSensor;

	}

	resetInputMode();
	return 0;
}

void speed_select(int &Kp, float &Ki, float &Kd) {

	std::cout << "Enter Kp. " << std::endl;

	while(!enterButton.isPressed()) {

		if(upButton.isPressed()) Kp = Kp + 10;
		if(downButton.isPressed()) Kp = Kp - 10;
		if(rightButton.isPressed()) Kp = Kp + 1;
		if(leftButton.isPressed()) Kp = Kp - 1; 
		if(Kp < 0) Kp = 0;
	}
	std::cout << "Kp = " << Kp << "." << std::endl;

	std::cout << "Enter Ki. " << std::endl;

	while(!enterButton.isPressed()) {

		if(upButton.isPressed()) Ki = Ki + .01;
		if(downButton.isPressed()) Ki = Ki - .01;

		if(rightButton.isPressed()) Ki += .001;
		if(leftButton.isPressed()) Ki -= .001;
		if(Ki < 0) Ki = 0; 
	}

	std::cout << "Ki = " << Ki << "." << std::endl;

	std::cout << "Enter Kd. " << std::endl;

	while(!enterButton.isPressed()) {

		if(upButton.isPressed()) Kd = Kd + .01;
		if(downButton.isPressed()) Kd = Kd - .01;
		if(rightButton.isPressed()) Kd = Kd + .001;
		if(leftButton.isPressed()) Kd = Kd - .001;
		if(Kd < 0) Kd = 0; 
	}

	std::cout << "Kd = " << Kd << "." << std::endl;

	sleep(1);

}

void compute_Ki(int &errorI, int error_array[]) {


	errorI -= error_array[i];
	errorI += err;
	error_array[i] = err;

	i = (i + 1) % 64;

}

		
			


