#include "GCodeHandler.h"
#include "BoardManager.h"
#include <NTPClient.h>
#include <WiFiS3.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <SD.h>
#include "RTC.h"


bool buttonStates[16];
unsigned long lastButtonPressTime[16];

//holds hall effect sensor values
int hallSensorValues[64];
bool hallSensorStates[64];

WiFiUDP Udp;  // A UDP instance to let us send and receive packets over UDP
NTPClient timeClient(Udp);

RTCTime currentTime;

GCodeHandler myGCodeHandler(Serial1, Serial);
BoardManager myBoardManager(Serial, myGCodeHandler, timeClient, currentTime, buttonStates, hallSensorValues, hallSensorStates);

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

  myBoardManager.initialize();
}

void loop() {
	RTC.getTime(currentTime);

	//check for button presses
	myBoardManager.updateButtonStates();
	for(int i = 0; i < NUM_BUTTONS; i++) {
		if(buttonStates[BUTTON_INDEX[i]] && (unsigned long)(millis() - lastButtonPressTime[i]) >= BUTTON_PRESS_COOLDOWN) {
			lastButtonPressTime[i] = millis();
			buttonPressed(i);
			break;
		}
	}

	//magic happens here
	myBoardManager.update();

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
		myBoardManager.finalizeToDos();
	} else if(buttonNum == 2) {
		//play a game button
		myBoardManager.ticTacToe();
	} else if(buttonNum == 3) {
		//evening update button
		myBoardManager.forceEveningUpdate();
	} else if(buttonNum == 4) {
		//daytime udpate button
		myBoardManager.forceDaytimeUpdate();
	} else if(buttonNum == 5) {
		//morning update button
		myBoardManager.forceMorningUpdate();
	} else if(buttonNum == 6) {
		//connect to bluetooth button
		myBoardManager.openBluetoothBLE();
	}
}