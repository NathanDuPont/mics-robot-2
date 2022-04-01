#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>
#include "Adafruit_TCS34725.h"
#include "constants.h"

/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

void printDebug(char* msg) {
	if (DEBUG) {
		Serial.print(msg);
	}
}

void printLnDebug(char* msg) {
	if (DEBUG) {
		Serial.print(msg);
	}
}

Servo rotationMotor;
Servo driverMotor;
bool ret;

void moveDriver(bool extend, int speed) {
	digitalWrite(DRIVER_IN1_PIN, extend);
	digitalWrite(DRIVER_IN2_PIN, !extend);
	driverMotor.write(speed);
}

void moveRotation(bool extend, int speed) {
	digitalWrite(ROTATION_IN1_PIN, !extend);
	digitalWrite(ROTATION_IN2_PIN, extend);
	rotationMotor.write(speed);
}

void setup(void) {
	pinMode(COLOR_LED_PIN, OUTPUT);
	pinMode(ROTATION_IN1_PIN, OUTPUT);
	pinMode(ROTATION_IN2_PIN, OUTPUT);
	pinMode(DRIVER_IN1_PIN, OUTPUT);
	pinMode(DRIVER_IN2_PIN, OUTPUT);
	
	rotationMotor.attach(ROTATION_PWM_PIN);
	driverMotor.attach(DRIVER_PWM_PIN);

	ret = false;

	digitalWrite(COLOR_LED_PIN, HIGH);

	Serial.begin(9600);

	if (tcs.begin()) {
	Serial.println("Found sensor");
	} else {
	Serial.println("No TCS34725 found ... check your connections");
	while (1);
	}
}

void loop(void) {
	// Variable initialization
	uint16_t r, g, b, c, colorTemp, lux;

	// Color sensor
	tcs.getRawData(&r, &g, &b, &c);
	// colorTemp = tcs.calculateColorTemperature(r, g, b);
	// colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
	lux = tcs.calculateLux(r, g, b);

	// Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
	// Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
	// Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
	// Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
	// Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
	// Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
	// Serial.println(" ");

	moveDriver(true, 180);
	tcs.getRawData(&r, &g, &b, &c);
	lux = tcs.calculateLux(r, g, b);

	delay(5000);

	while (lux > 200) {
		moveDriver(true, 180);
		tcs.getRawData(&r, &g, &b, &c);
		lux = tcs.calculateLux(r, g, b);
	}


	moveDriver(false, 180);
	tcs.getRawData(&r, &g, &b, &c);
	lux = tcs.calculateLux(r, g, b);

	delay(5000);

	while (lux > 200) {
		moveDriver(false, 180);
		tcs.getRawData(&r, &g, &b, &c);
		lux = tcs.calculateLux(r, g, b);
		Serial.println(lux, DEC);
	}

	exit(0);

	// if (!ret && lux > 25) {
	// 	moveDriver(true, 180);
	// 	Serial.println("Case 1");
	// } else if (ret && lux > 120) {
	// 	moveDriver(false, 180);
	// 	Serial.println("Case 2");
	// } else if (ret) {
	// 	moveDriver(false, 0);
	// 	delay(200);
	// 	moveRotation(false, 90);
	// 	delay(300);
	// 	moveRotation(false, 0);
	// 	exit(0);
	// 	Serial.println("Case 3");
	// } else {
	// 	moveDriver(true, 0);
	// 	delay(200);
	// 	moveRotation(true, 90);
	// 	delay(300);
	// 	moveRotation(true, 0);
	// 	ret = true;
	// 	moveDriver(false, 90);
	// 	delay(200);
	// 	Serial.println("Case 4");
	// }

	// moveDriver(true, 180);
	// delay(35000);
	// moveDriver(false, 180);
	// delay(19000);
	// exit(0);
}