#include "GCodeHandler.h"

//Selectors choose which multiplexer channel to read
const int MULTI_SELECT0 = 5;
const int MULTI_SELECT1 = 4;
const int MULTI_SELECT2 = 3;
const int MULTI_SELECT3 = 2;

//Multiplexer signal pins
const int SIGNAL_BUTTON_MULTI = A0;
const int SIGNAL_HALL_MULTI1 = A1;
const int SIGNAL_HALL_MULTI2 = A2;
const int SIGNAL_HALL_MULTI3 = A3;
const int SIGNAL_HALL_MULTI4 = A4;

//Servo pins
const int SERVO_1_ENABLE = 6;
const int SERVO_2_ENABLE = 7;
const int SERVO_3_ENABLE = 8;
const int SERVO_SIGNAL = 9;

//holds button states
boolean buttonStates[16];
//holds hall effect sensor values
int hallSensorValues[64];

GCodeHandler myGCodeHandler(Serial1, Serial);

void setup() {
	//Serial is used to communicate with the console
	Serial.begin(115200);
	//Serial1 is used to communicate with the CNC arduino
	Serial1.begin(115200);
	delay(100);

	pinMode(MULTI_SELECT0, OUTPUT);
	pinMode(MULTI_SELECT1, OUTPUT);
	pinMode(MULTI_SELECT2, OUTPUT);
	pinMode(MULTI_SELECT3, OUTPUT);

	pinMode(SIGNAL_BUTTON_MULTI, INPUT);
	pinMode(SIGNAL_HALL_MULTI1, INPUT);
	pinMode(SIGNAL_HALL_MULTI2, INPUT);
	pinMode(SIGNAL_HALL_MULTI3, INPUT);
	pinMode(SIGNAL_HALL_MULTI4, INPUT);

	pinMode(SERVO_1_ENABLE, OUTPUT);
	pinMode(SERVO_2_ENABLE, OUTPUT);
	pinMode(SERVO_3_ENABLE, OUTPUT);
	pinMode(SERVO_SIGNAL, OUTPUT);

	myGCodeHandler.initialize();

	digitalWrite(SERVO_1_ENABLE, HIGH);
	digitalWrite(SERVO_2_ENABLE, LOW);
	digitalWrite(SERVO_3_ENABLE, LOW);
}

void loop() {
	Serial.println("test");
	delay(2000);
	myGCodeHandler.sendSingleCommand("G10 P0 L20 X0 Y0 Z0.2");
	myGCodeHandler.setCursor(100, 480);
	myGCodeHandler.setFontScale(1.2);
	myGCodeHandler.sendWord("Good morning, Doug!");
	myGCodeHandler.setCursor(100, 420);
	myGCodeHandler.setFontScale(0.6);
	myGCodeHandler.sendWord("Wed, February 14, 2024");
	delay(400000);
}

//reads all button states in just over 1ms
void updateButtonStates() {
	for(int i = 0; i < 16; i++) {
    digitalWrite(MULTI_SELECT0, bitRead(i, 0));
    digitalWrite(MULTI_SELECT1, bitRead(i, 1));
    digitalWrite(MULTI_SELECT2, bitRead(i, 2));
    digitalWrite(MULTI_SELECT3, bitRead(i, 3));
    delayMicroseconds(63);

		buttonStates[i] = digitalRead(SIGNAL_BUTTON_MULTI);
  }
}

//reads all hall effect sensor values in just over 1ms
void updateHallSensorValues() {
	for(int i = 0; i < 16; i++) {
    digitalWrite(MULTI_SELECT0, bitRead(i, 0));
    digitalWrite(MULTI_SELECT1, bitRead(i, 1));
    digitalWrite(MULTI_SELECT2, bitRead(i, 2));
    digitalWrite(MULTI_SELECT3, bitRead(i, 3));
    delayMicroseconds(63);

		hallSensorValues[i] = analogRead(SIGNAL_HALL_MULTI1);
		hallSensorValues[i + 16] = analogRead(SIGNAL_HALL_MULTI2);
		hallSensorValues[i + 32] = analogRead(SIGNAL_HALL_MULTI3);
		hallSensorValues[i + 48] = analogRead(SIGNAL_HALL_MULTI4);
  }
}