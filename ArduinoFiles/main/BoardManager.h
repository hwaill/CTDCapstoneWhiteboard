#ifndef BoardManagerClass

#define BoardManagerClass
#include "Arduino.h"
#include "GCodeHandler.h"
#include <NTPClient.h>
#include <WiFiS3.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <SD.h>
#include "RTC.h"
#include <ArduinoBLE.h>

struct ToDoListItem {
	char name[50];
	char* label;
};

struct CalendarEvent {
	char* name;
	unsigned long epochTime;
};

inline const double TODO_LINE_HEIGHT = 37;
inline const double TODO_ITEM_FONT_SCALE = 0.6;
inline const double TODO_LABEL_FONT_SCALE = 0.5;
inline const double TODO_CURSOR_OFFSET_X = 9;
inline const double TODO_ITEM_WIDTH = 286;
inline const double TODO_LEFT_LABEL_SPACE = 40;
inline const double TODO_CHECKBOX_SPACE = 37;

inline const double TODO_Y_START = 487.727;
inline const double TODO_LEFT_X_START = 33.282;
inline const double TODO_RIGHT_X_START = 348.207;

inline const char* MONTH_LONG[12] = {
  "January",
  "February",
  "March",
  "April",
  "May",
  "June",
  "July",
  "August",
  "September",
  "October",
  "November",
  "December"
};

inline const char* DAY[7] {
  "Sunday",
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday"
};

class BoardManager {
	public:
		BoardManager(Stream &consoleSerial, GCodeHandler &myGCodeHandler, NTPClient &timeClient, RTCTime &currentTime, bool *buttonStates, int *hallSensorValues);
		void initialize();
		void updateFromConfig();

		char* getWifiSSID();
		char* getWifiPass();

		void openBluetoothBLE();

		void NTP();
		unsigned long lastTimeUpdate;

		ToDoListItem _morningToDoList[20];
		ToDoListItem _dayToDoList[20];
		ToDoListItem _eveningToDoList[20];

		int _numMorningToDos;
		int _numDayToDos;
		int _numEveningToDos;

		void drawListSection(double startY, double startX, int numItems, char* listName, ToDoListItem *itemList, bool hasCheckboxes, bool hasLeftLabel);
	private:
		Stream* _consoleSerial;
		GCodeHandler* _myGCodeHandler;

		bool* _buttonStates;
		int* _hallSensorValues;

		bool _needsBluetoothConfig = false;

		bool _hasWiFiInfo = false;
		char _wifiSSID[30];
		char _wifiPass[30];
		int _wifiStatus = WL_IDLE_STATUS;

		RTCTime* _currentTime;
		NTPClient* _timeClient;
		int _timeZoneOffsetHours = -7;

		bool _connectToWifi();
		void _checkForWifiInfo();
		void _printWifiStatus();

		char _userFirstName[20];
		char _userLastName[20];
		
		int _theme;

		int _numFeatures;
		bool _features[20];

		int _numMoodQuestions;
		bool _moodQuestions[10];

		char _zipcode[6];
};

#endif