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
	char* name;
};

struct CalendarEvent {
	char* name;
	unsigned long epochTime;
};

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
	private:
		Stream* _consoleSerial;
		GCodeHandler* _myGCodeHandler;

		bool* _buttonStates;
		int* _hallSensorValues;

		char* _userFirstName;
		char* _userLastName;

		bool _needsBluetoothConfig = false;

		bool _hasWiFiInfo = false;
		char* _wifiSSID;
		char* _wifiPass;
		int _wifiStatus = WL_IDLE_STATUS;

		RTCTime* _currentTime;
		NTPClient* _timeClient;
		int _timeZoneOffsetHours = -7;

		bool _connectToWifi();
		void _checkForWifiInfo();
		void _printWifiStatus();

		ToDoListItem _morningToDoList[20];
		ToDoListItem _dayToDoList[20];
		ToDoListItem _eveningToDoList[20];

		int _numMorningToDos;
		int _numDayToDos;
		int _numEveningToDos;
		
		int _theme;

		bool _featureToDo;
		bool _featureWeeklyCalendar;
		bool _featureDailyQuote;
		bool _featureWeather;
		bool _featureTicTacToe;
		bool _featureMoodTracker;
		
};

#endif