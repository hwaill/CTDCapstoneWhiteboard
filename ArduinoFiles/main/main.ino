#include "GCodeHandler.h"
#include "BoardManager.h"
#include <NTPClient.h>
#include <WiFiS3.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <SD.h>
#include "RTC.h"

//Selectors choose which multiplexer channel to read
const int MULTI_SELECT0 = 2;
const int MULTI_SELECT1 = 3;
const int MULTI_SELECT2 = 4;
const int MULTI_SELECT3 = 5;

//Multiplexer signal pins
const int SIGNAL_BUTTON_MULTI = A0;
const int SIGNAL_HALL_MULTI1 = A4;
const int SIGNAL_HALL_MULTI2 = A3;
const int SIGNAL_HALL_MULTI3 = A2;
const int SIGNAL_HALL_MULTI4 = A1;

//Servo pins
const int SERVO_1_ENABLE = 6;
const int SERVO_2_ENABLE = 7;
const int SERVO_SIGNAL = 9;

const int LED_PIN = 8;

//SD Chip Select Pin
const int SD_CS_PIN = 10;

//holds button states
const int NUM_BUTTONS = 7;
const int BUTTON_PRESS_COOLDOWN = 750;
bool buttonStates[16];
unsigned long lastButtonPressTime[16];
const int BUTTON_INDEX[16] = {8, 9, 10, 11, 12, 13, 14, 15, 7, 6, 5, 4, 3, 2, 1};

//holds hall effect sensor values
int hallSensorValues[64];
bool hallSensorStates[64];

WiFiUDP Udp;  // A UDP instance to let us send and receive packets over UDP
NTPClient timeClient(Udp);

RTCTime currentTime;

GCodeHandler myGCodeHandler(Serial1, Serial);
BoardManager myBoardManager(Serial, myGCodeHandler, timeClient, currentTime, buttonStates, hallSensorValues);

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
	pinMode(SERVO_SIGNAL, OUTPUT);

	pinMode(LED_PIN, OUTPUT);

	pinMode(SD_CS_PIN, OUTPUT);

	digitalWrite(SERVO_1_ENABLE, HIGH);
	digitalWrite(SERVO_2_ENABLE, HIGH);

	for(int i = 0; i < 16; i++) {
		lastButtonPressTime[i] = millis();
	}

	needsMorningUpdate = true;
	needsDaytimeUpdate = true;
	needsEveningUpdate = true;

  myBoardManager.initialize();
}

void loop() {
	RTC.getTime(currentTime);

	//check for button presses
	updateButtonStates();
	for(int i = 0; i < NUM_BUTTONS; i++) {
		if(buttonStates[BUTTON_INDEX[i]] && (unsigned long)(millis() - lastButtonPressTime[i]) >= BUTTON_PRESS_COOLDOWN) {
			lastButtonPressTime[i] = millis();
			buttonPressed(i);
			break;
		}
	}

	myBoardManager.update();

  // Serial.print(DAY[DayOfWeek2int(currentTime.getDayOfWeek(), true) - 1]);
  // Serial.print(", ");
  // Serial.print(MONTH_LONG[Month2int(currentTime.getMonth()) - 1]);
  // Serial.print(" ");
  // Serial.print(currentTime.getDayOfMonth());
  // Serial.print(", ");
  // Serial.print(currentTime.getYear());
  // Serial.print(" ");

  Serial.print(currentTime.getHour());
  // Serial.print(":");
  // Serial.print(currentTime.getMinutes());
  // Serial.print(":");
  // Serial.println(currentTime.getSeconds());
	// myBoardManager.drawListSection(400, 60, myBoardManager._numMorningToDos, "Morning Tasks", myBoardManager._morningToDoList, true, false);

	//update RTC from internet
	if((unsigned long)(millis() - myBoardManager.lastTimeUpdate) > 600000) {
		myBoardManager.NTP();
	}
}

void buttonPressed(int buttonNum) {
	if(buttonNum == 0) {
		//pause board button
		myBoardManager.togglePaused();
	} else if(buttonNum == 1) {
		//finalize to-dos button
		
	} else if(buttonNum == 2) {
		//play a game button
		
	} else if(buttonNum == 3) {
		//evening update button
		
	} else if(buttonNum == 4) {
		//daytime udpate button
		
	} else if(buttonNum == 5) {
		//morning update button
		
	} else if(buttonNum == 6) {
		//connect to bluetooth button

	}
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

	for(int i = 0; i < 64; i++) {
		hallSensorStates[i] = hallSensorValues[i] > 560 || hallSensorValues[i] < 440;
	}
}