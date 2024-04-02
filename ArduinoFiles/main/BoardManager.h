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
	char name[50];
	unsigned long epochTime;
};

//Selectors choose which multiplexer channel to read
inline const int MULTI_SELECT0 = 2;
inline const int MULTI_SELECT1 = 3;
inline const int MULTI_SELECT2 = 4;
inline const int MULTI_SELECT3 = 5;

//Multiplexer signal pins
inline const int SIGNAL_BUTTON_MULTI = A0;
inline const int SIGNAL_HALL_MULTI1 = A4;
inline const int SIGNAL_HALL_MULTI2 = A3;
inline const int SIGNAL_HALL_MULTI3 = A2;
inline const int SIGNAL_HALL_MULTI4 = A1;

//Servo pins
inline const int SERVO_1_ENABLE = 6;
inline const int SERVO_2_ENABLE = 7;
inline const int SERVO_SIGNAL = 9;

inline const int LED_PIN = 8;

//SD Chip Select Pin
inline const int SD_CS_PIN = 10;

inline const int LEFT_TODO_SENSORS[12] = {};
inline const int RIGHT_TODO_SENSORS[12] = {};
inline const int GRID_SENSORS[5][5] = {{}, {}, {}, {}, {}};
inline const int LEFT_FOUR_SENSORS[4] = {};
inline const int RIGHT_FOUR_SENSORS[4] = {};

//holds button states
inline const int NUM_BUTTONS = 7;
inline const int BUTTON_PRESS_COOLDOWN = 750;
inline const int BUTTON_INDEX[16] = {8, 9, 10, 11, 12, 13, 14, 15, 7, 6, 5, 4, 3, 2, 1};

inline const unsigned long USER_INTERACTION_WAIT_COOLDOWN = 900000;

//board layout variables
//for error messages
inline const double ERROR_FONT_SCALE = 1.2;
inline const double ERROR_START_X = 60;
inline const double ERROR_START_Y = 400;

//for titles and subtitles
inline const double TITLE_FONT_SCALE = 2;
inline const double TITLE_START_X = 30;
inline const double TITLE_START_Y = 510;
inline const double SUBTITLE_FONT_SCALE = 1.2;
inline const double SUBTITLE_START_X = 30;
inline const double SUBTITLE_START_Y = 475;

//for todo lists
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

inline const int UPDATE_TYPE_NONE = 0;
inline const int UPDATE_TYPE_MORNING = 1;
inline const int UPDATE_TYPE_DAYTIME = 2;
inline const int UPDATE_TYPE_EVENING = 3;
inline const int UPDATE_TYPE_OTHER = 4;

class BoardManager {
	public:
		BoardManager(Stream &consoleSerial, GCodeHandler &myGCodeHandler, NTPClient &timeClient, RTCTime &currentTime, bool *buttonStates, int *hallSensorValues, bool *hallSensorStates);
		void initialize();
		void updateFromConfig();

		void update();

		void togglePaused();

		void morningUpdate();
		void daytimeUpdate();
		void eveningUpdate();

		void forceMorningUpdate();
		void forceDaytimeUpdate();
		void forceEveningUpdate();

		void ticTacToe();

		//for tictactoe
		bool ticTacToe_checkWin(char player);
		bool ticTacToe_isBoardFull();
		void ticTacToe_computerMove();

		const char TICTACTOE_COMPUTER = 'X';
		const char TICTACTOE_PLAYER = 'O'; 
		const char TICTACTOE_EMPTY = ' ';
		
		char ticTacToe_board[3][3];
		
		int ticTacToe_sensorGridIndexes[3][3] = {
		    {GRID_SENSORS[0][0], GRID_SENSORS[0][2], GRID_SENSORS[0][4]}, 
		    {GRID_SENSORS[2][0], GRID_SENSORS[2][2], GRID_SENSORS[2][4]}, 
		    {GRID_SENSORS[4][0], GRID_SENSORS[4][2], GRID_SENSORS[4][4]} 
		};

		bool ticTacToe_sensorGridStates[3][3] = {
			{0, 0, 0},
			{0, 0, 0},
			{0, 0, 0}
		};
		//for tictactoe

		void finalizeToDos();

		void captureToDos();

		char* getWifiSSID();
		char* getWifiPass();

		void openBluetoothBLE();

		void updateButtonStates();
		void updateHallEffectStates();

		void NTP();
		unsigned long lastTimeUpdate;

		ToDoListItem _morningToDoList[20];
		ToDoListItem _dayToDoList[20];
		ToDoListItem _eveningToDoList[20];
		ToDoListItem _weeklyToDoList[20];

		CalendarEvent _events[20];

		int _numMorningToDos;
		int _numDayToDos;
		int _numEveningToDos;
		int _numWeeklyToDos;

		int _numEvents;

		void drawListSection(double startY, double startX, int numItems, char* listName, ToDoListItem *itemList, bool hasCheckboxes, bool hasLeftLabel);
		void drawWeather();
		void drawQuote();
		void drawMorningMoodQs();
		void drawEveningMoodQs();

		void updateRewards();
	private:
		Stream* _consoleSerial;
		GCodeHandler* _myGCodeHandler;

		bool* _buttonStates;
		bool* _hallSensorStates;
		int* _hallSensorValues;

		unsigned long _lastUserInteractionTime;

		bool _isPaused = false;
		int _lastUpdateType = UPDATE_TYPE_NONE;

		bool _needsBluetoothConfig = false;

		bool _hasWiFiInfo = false;
		char _wifiSSID[30];
		char _wifiPass[30];
		int _wifiStatus = WL_IDLE_STATUS;

		RTCTime* _currentTime;
		NTPClient* _timeClient;
		int _timeZoneOffsetHours = -7;

		unsigned long _currentDay;

		bool _needsMorningUpdate = true;
		bool _needsDaytimeUpdate = true;
		bool _needsEveningUpdate = true;

		bool _connectToWifi();
		void _printWifiStatus();
		void _displayError(const char* errorMessage, int lineNumber);

		char _userFirstName[20];
		char _userLastName[20];
		
		int _theme;
		
		//features:
		//0 = todolist
		//1 = events
		//2 = mood
		//3 = weather
		//4 = game
		//5 = quotes
		//6 = theme
		int _numFeatures;
		bool _features[20];

		int _numMoodQuestions;
		bool _moodQuestions[10];
		
		char _latitude[12];
		char _longitude[12];
};

#endif
