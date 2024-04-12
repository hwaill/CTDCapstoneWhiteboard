#include "GCodeHandler.h"
#include "BoardManager.h"
#include <NTPClient.h>
#include <WiFiS3.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <SD.h>
#include "RTC.h"

WiFiUDP Udp;  // A UDP instance to let us send and receive packets over UDP
NTPClient timeClient(Udp);

RTCTime currentTime;

GCodeHandler myGCodeHandler(Serial1, Serial);
BoardManager myBoardManager(Serial, myGCodeHandler, timeClient, currentTime);

void setup() {
	//Serial is used to communicate with the console
	Serial.begin(115200);
	//Serial1 is used to communicate with the CNC arduino
	Serial1.begin(115200);
	delay(100);

	pinMode(PIN_MULTIPLEXER_S0, OUTPUT);
	pinMode(PIN_MULTIPLEXER_S1, OUTPUT);
	pinMode(PIN_MULTIPLEXER_S2, OUTPUT);
	pinMode(PIN_MULTIPLEXER_S3, OUTPUT);

	pinMode(PIN_MULTIPLEXER_SIGNAL_BUTTON, INPUT);
	pinMode(PIN_MULTIPLEXER_SIGNAL_HALL1, INPUT);
	pinMode(PIN_MULTIPLEXER_SIGNAL_HALL2, INPUT);
	pinMode(PIN_MULTIPLEXER_SIGNAL_HALL3, INPUT);
	pinMode(PIN_MULTIPLEXER_SIGNAL_HALL4, INPUT);

	pinMode(PIN_SERVO_1_ENABLE, OUTPUT);
	pinMode(PIN_SERVO_2_ENABLE, OUTPUT);
	pinMode(PIN_SERVO_SIGNAL, OUTPUT);

	pinMode(PIN_INDICATOR_LED, OUTPUT);

	pinMode(PIN_SD_CHIP_SELECT, OUTPUT);

	digitalWrite(PIN_SERVO_1_ENABLE, HIGH);
	digitalWrite(PIN_SERVO_2_ENABLE, HIGH);

  myBoardManager.initialize();
}

void loop() {
	RTC.getTime(currentTime);

	//magic happens here
	myBoardManager.update();

	//update RTC from internet
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