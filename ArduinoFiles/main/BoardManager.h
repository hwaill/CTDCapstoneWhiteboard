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
#include <Arduino_JSON.h>
#include <assert.h>

struct ToDoListItem {
	char name[50];
	char* label;
};

//Selectors choose which multiplexer channel to read
inline const int PIN_MULTIPLEXER_S0 = 2;
inline const int PIN_MULTIPLEXER_S1 = 3;
inline const int PIN_MULTIPLEXER_S2 = 4;
inline const int PIN_MULTIPLEXER_S3 = 5;

//Multiplexer signal pins
inline const int PIN_MULTIPLEXER_SIGNAL_BUTTON = A0;
inline const int PIN_MULTIPLEXER_SIGNAL_HALL1 = A4;
inline const int PIN_MULTIPLEXER_SIGNAL_HALL2 = A3;
inline const int PIN_MULTIPLEXER_SIGNAL_HALL3 = A2;
inline const int PIN_MULTIPLEXER_SIGNAL_HALL4 = A1;

//Servo pins
inline const int PIN_SERVO_1_ENABLE = 6;
inline const int PIN_SERVO_2_ENABLE = 7;

//alternate servo signal for idle servo
inline const int PIN_SERVO_SIGNAL = 9;

//front LED output
inline const int PIN_INDICATOR_LED = 8;

//SD Chip Select Pin
inline const int PIN_SD_CHIP_SELECT = 10;

//indices for
inline const int LEFT_TODO_SENSORS[12] = {6, 4, 5, 3, 7, 2, 20, 22, 1, 23, 0, 21};
inline const int RIGHT_TODO_SENSORS[12] = {63, 60, 62, 59, 61, 58, 52, 50, 48, 49, 51, 53};
inline const int GRID_SENSORS[5][5] = {{11, 19, 29, 15, 41}, {8, 17, 31, 26, 40}, {9, 18, 27, 14, 44}, {10, 39, 28, 24, 43}, {12, 16, 30, 13, 42}};
inline const int LEFT_FOUR_SENSORS[4] = {57, 47, 46, 45};
inline const int RIGHT_FOUR_SENSORS[4] = {32, 33, 54, 55};

//holds button states
inline const int NUM_BUTTONS = 7;
inline const int BUTTON_PRESS_COOLDOWN = 750;
inline const int BUTTON_INDEX[16] = {8, 9, 10, 11, 12, 13, 14, 15, 7, 6, 5, 4, 3, 2, 1};

inline const unsigned long USER_INTERACTION_WAIT_COOLDOWN = 900000;

//board layout variables

//for error messages
inline const double LAYOUT_ERROR_FONT_SCALE = 1.2;
inline const double LAYOUT_ERROR_START_X = 60;
inline const double LAYOUT_ERROR_START_Y = 460;

//for titles and subtitles
inline const double LAYOUT_TITLE_FONT_SCALE = 1;
inline const double LAYOUT_TITLE_START_X = 30;
inline const double LAYOUT_TITLE_START_Y = 510;
inline const double LAYOUT_SUBTITLE_FONT_SCALE = 0.7;
inline const double LAYOUT_SUBTITLE_START_X = 30;
inline const double LAYOUT_SUBTITLE_START_Y = 458;

//for todo lists
inline const int LIST_LEFT = 0;
inline const int LIST_RIGHT = 1;

inline const double LAYOUT_TODO_LINE_HEIGHT = 30;
inline const double LAYOUT_TODO_ITEM_FONT_SCALE = 0.6;
inline const double LAYOUT_TODO_LABEL_FONT_SCALE = 0.5;
inline const double LAYOUT_TODO_CURSOR_OFFSET_X = 9;
inline const double LAYOUT_TODO_ITEM_WIDTH = 286;
inline const double LAYOUT_TODO_LEFT_LABEL_SPACE = 40;
inline const double LAYOUT_TODO_CHECKBOX_SPACE = 37;

inline const double LAYOUT_FIRST_SENSOR_Y = 426.7;
inline const double LAYOUT_LEFT_SENSOR_X = 287;
inline const double LAYOUT_RIGHT_SENSOR_X = 607;
inline const double LAYOUT_TODO_Y_START = 487.727;
inline const double LAYOUT_TODO_LEFT_X_START = 33;
inline const double LAYOUT_TODO_RIGHT_X_START = 315;

//for converting dates to strings
inline const char* MONTH_LONG[12] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
inline const char* DAY[7] {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

//keeping track of board state (most recent update)
inline const int UPDATE_TYPE_NONE = 0;
inline const int UPDATE_TYPE_MORNING = 1;
inline const int UPDATE_TYPE_DAYTIME = 2;
inline const int UPDATE_TYPE_EVENING = 3;
inline const int UPDATE_TYPE_OTHER = 4;

class BoardManager {
	public:
  	BoardManager(Stream &consoleSerial, GCodeHandler &myGCodeHandler, NTPClient &timeClient, RTCTime &currentTime);
		
		void initialize();
		void update();
		void updateConfigFromSD();

		void http_request(float latitude, float longitude);

		void buttonPressed(int buttonIndex);
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

		int _numMorningToDos;
		int _numDayToDos;
		int _numEveningToDos;
		int _numWeeklyToDos;

		int _numEvents;

		void drawListSection(double rowStart, int leftOrRight, int numItems, char* listName, ToDoListItem *itemList, bool hasCheckboxes, bool hasLeftLabel);
		bool getWeather(); 
		void drawWeather();
		void drawQuote();
		void drawMorningMoodQs();
		void drawEveningMoodQs();

		void updateRewards();
	private:
		Stream* _consoleSerial;
		GCodeHandler* _myGCodeHandler;
		RTCTime* _currentTime;
		NTPClient* _timeClient;

		JSONVar myObject;
		int temperature = 65;

		bool _buttonStates[16];
		bool _hallSensorStates[64];
		int _hallSensorValues[64];
		unsigned long _lastButtonPressTimes[16];

		unsigned long _lastUserInteractionTime;

		bool _isPaused = false;
		int _lastUpdateType = UPDATE_TYPE_NONE;

		bool _needsBluetoothConfig = false;

		bool _hasWiFiInfo = false;
		char _wifiSSID[30];
		char _wifiPass[30];
		int _wifiStatus = WL_IDLE_STATUS;
		int _timeZoneOffsetHours = -7;

		//for tracking when updates are needed
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
