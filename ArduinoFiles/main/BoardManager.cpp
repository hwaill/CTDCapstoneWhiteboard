#include "BoardManager.h"

void bleConnectHandler(BLEDevice central);
void bleDisconnectHandler(BLEDevice central);

BoardManager::BoardManager(Stream &consoleSerial, GCodeHandler &myGCodeHandler, NTPClient &timeClient, RTCTime &currentTime) {
	_consoleSerial = &consoleSerial;
	_myGCodeHandler = &myGCodeHandler;
	_timeClient = &timeClient;
	_currentTime = &currentTime;

	// _buttonStates = buttonStates;
	// _hallSensorValues = hallSensorValues;
	// _hallSensorStates = hallSensorStates;
	// _lastButtonPressTimes = lastButtonPressTimes;
}

void BoardManager::initialize() {
	_lastUserInteractionTime = millis() - USER_INTERACTION_WAIT_COOLDOWN;
	_myGCodeHandler->initialize();
	updateConfigFromSD();

	if(_hasWiFiInfo) {
		_connectToWifi();
	} else {
		_displayError("Wifi credentials required!", 0);
		_displayError("Please configure with bluetooth.", 1);
		_myGCodeHandler->returnToHome();
	}
	RTC.begin();

	do {
		_consoleSerial->println("\nStarting connection to server...");
		_timeClient->begin();
		_timeClient->update();

		// Get the current time from NTP
	
		NTP();
		RTC.getTime(*_currentTime);
		_consoleSerial->println(_currentTime->toString());
		_currentDay = _currentTime->getUnixTime() / 86400UL;
	} while(_currentTime->getYear() > 2100);

	// FIXME: Get weather working
	// while(!getWeather()) {};

	digitalWrite(PIN_INDICATOR_LED, HIGH);
	delay(400);
	digitalWrite(PIN_INDICATOR_LED, LOW);
	delay(400);
	digitalWrite(PIN_INDICATOR_LED, HIGH);
	delay(400);
	digitalWrite(PIN_INDICATOR_LED, LOW);
	delay(400);
	digitalWrite(PIN_INDICATOR_LED, HIGH);
	delay(400);
	digitalWrite(PIN_INDICATOR_LED, LOW);
}

void BoardManager::update() {
	updateButtonStates();
	for(int i = 0; i < NUM_BUTTONS; i++) {
		if(_buttonStates[BUTTON_INDEX[i]] && (unsigned long)(millis() - _lastButtonPressTimes[i]) >= BUTTON_PRESS_COOLDOWN) {
			_lastButtonPressTimes[i] = millis();
			buttonPressed(i);
			break;
		}
	}

	if((unsigned long)(millis() - lastTimeUpdate) > 600000) {
		NTP();
	}

	//checks if it is a new day (at midnight)
	if(!_isPaused) {
		if(_currentDay != _currentTime->getUnixTime() / 86400UL) {
			_currentDay = _currentTime->getUnixTime() / 86400UL;

			_needsMorningUpdate = true;
			_needsDaytimeUpdate = true;
			_needsEveningUpdate = true;
		}

		if((unsigned long)(millis() - _lastUserInteractionTime) >= USER_INTERACTION_WAIT_COOLDOWN) {
			if(_needsMorningUpdate && _currentTime->getHour() >= 7 && _currentTime->getHour() < 12) {
				_needsMorningUpdate = false;
				morningUpdate();
			} else if(_needsDaytimeUpdate && _currentTime->getHour() >= 12 && _currentTime->getHour() < 18) {
				_needsDaytimeUpdate = false;
				daytimeUpdate();
			} else if(_needsEveningUpdate && _currentTime->getHour() >= 18 && _currentTime->getHour() < 23) {
				_needsEveningUpdate = false;
				eveningUpdate();
			}
		}
	}
}

void BoardManager::buttonPressed(int buttonNum) {
	if(buttonNum == 0) {
		//pause board button
		togglePaused();
	} else if(buttonNum == 1) {
		//finalize to-dos button
		finalizeToDos();
	} else if(buttonNum == 2) {
		//play a game button
		ticTacToe();
	} else if(buttonNum == 3) {
		//evening update button
		forceEveningUpdate();
	} else if(buttonNum == 4) {
		//daytime udpate button
		forceDaytimeUpdate();
	} else if(buttonNum == 5) {
		//morning update button
		forceMorningUpdate();
	} else if(buttonNum == 6) {
		//connect to bluetooth button
		openBluetoothBLE();
	}
}

void BoardManager::morningUpdate() {
	//check previous to dos

	// TODO: Revisit what to do for previous update
	if(_lastUpdateType != UPDATE_TYPE_NONE && _lastUpdateType != UPDATE_TYPE_OTHER) {
		updateRewards();
	}
	//print good morning
	String text = "Good morning, ";
	text += _userFirstName;
	text += "!";
	_myGCodeHandler->setCursor(LAYOUT_TITLE_START_X, LAYOUT_TITLE_START_Y);
	_myGCodeHandler->setFontScale(LAYOUT_TITLE_FONT_SCALE);
	_myGCodeHandler->write(text, WRAP_TRUNCATE, false);
	//print the date
	text = "It's ";
	text += DAY[DayOfWeek2int(_currentTime->getDayOfWeek(), true) - 1];
	text += ", ";
	text += MONTH_LONG[Month2int(_currentTime->getMonth()) - 1];
	text += " ";
	text += _currentTime->getDayOfMonth();
  text += ", ";
  text += _currentTime->getYear();
	_myGCodeHandler->setCursor(LAYOUT_SUBTITLE_START_X, LAYOUT_SUBTITLE_START_Y);
	_myGCodeHandler->setFontScale(LAYOUT_SUBTITLE_FONT_SCALE);
	_myGCodeHandler->write(text, WRAP_TRUNCATE, false);
	//print todos
	drawListSection(1, LIST_LEFT, _numMorningToDos, "Morning To-Do", _morningToDoList, true, false);
	//print events?
	//drawListSection(TODO_Y_START, TODO_RIGHT_X_START, _numWeeklyToDos, "Weekly To-Do", _weeklyToDoList, true, false);
	//print weather
	drawWeather();
	//print quote
	drawQuote();
	//print sleep survey
	drawMorningMoodQs();

	_myGCodeHandler->returnToHome();

	_lastUpdateType = UPDATE_TYPE_MORNING;
}

void BoardManager::forceMorningUpdate() {
	_lastUserInteractionTime = millis();
	morningUpdate();
}

// TODO: Daytime Update
void BoardManager::daytimeUpdate() {
}

void BoardManager::forceDaytimeUpdate() {
	_lastUserInteractionTime = millis();
	daytimeUpdate();
}

// TODO: Evening Update
void BoardManager::eveningUpdate() {

}

void BoardManager::forceEveningUpdate() {
	_lastUserInteractionTime = millis();
	eveningUpdate();
}

void BoardManager::togglePaused() {
	_isPaused = !_isPaused;
}

void BoardManager::ticTacToe() {
	//check board is empty of magnets
	// TODO: Move magnets away
	updateHallEffectStates();
	for(int row = 0; row < 3; row++) {
		for(int col = 0; col < 3; col++) {
			ticTacToe_sensorGridStates[row][col] = _hallSensorStates[ticTacToe_sensorGridIndexes[row][col]];
			if(ticTacToe_sensorGridStates[row][col] == 1) {
				return;
			}
		}
	}

	//reset board state
	for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ticTacToe_board[i][j] = TICTACTOE_EMPTY;
    }
  }
	
	_myGCodeHandler->drawLine(713, 24.5, 713, 232.7);
	_myGCodeHandler->drawLine(792.5, 24.5, 792.5, 232.7);
	_myGCodeHandler->drawLine(647.142, 90, 853.38, 90);
	_myGCodeHandler->drawLine(647.142, 170, 853.38, 170);

	_myGCodeHandler->sendSingleCommand("G00 X611 Y307");
	
	bool gameRunning = true;
	bool playerTurn = true;

	//game loop
	while(gameRunning) {
		if(playerTurn) {
			updateHallEffectStates();
			//update magnet sensors
			for(int row = 0; row < 3; row++) {
				for(int col = 0; col < 3; col++) {
					ticTacToe_sensorGridStates[row][col] = _hallSensorStates[ticTacToe_sensorGridIndexes[row][col]];
				}
			}

			for(int row = 0; row < 3; row++) {
				for(int col = 0; col < 3; col++) {
					if(ticTacToe_sensorGridStates[row][col] && ticTacToe_board[row][col] == TICTACTOE_EMPTY) {
						delay(1000);
						updateHallEffectStates();
						if(_hallSensorStates[ticTacToe_sensorGridIndexes[row][col]]) {
							ticTacToe_board[row][col] = TICTACTOE_PLAYER;
							playerTurn = false;
						}
					}
				}
			}
		} else {
			//computer's turn
			ticTacToe_computerMove();
			playerTurn = true;
		}

		// TODO: Improve win message layout
		if(ticTacToe_checkWin(TICTACTOE_PLAYER)) {
			_myGCodeHandler->setCursor(670,250);
			_myGCodeHandler->setFontScale(0.8);
			_myGCodeHandler->write("You win", WRAP_TRUNCATE, false);
			gameRunning = false;
		} else if(ticTacToe_checkWin(TICTACTOE_COMPUTER)) {
			_myGCodeHandler->setCursor(670,250);
			_myGCodeHandler->setFontScale(0.8);
			_myGCodeHandler->write("You lose", WRAP_TRUNCATE, false);
			gameRunning = false;
		} else if(ticTacToe_isBoardFull()) {
			_myGCodeHandler->setCursor(678.495,250);
			_myGCodeHandler->setFontScale(1);
			_myGCodeHandler->write("Draw", WRAP_TRUNCATE, false);
			gameRunning = false;
		}
	}

	_myGCodeHandler->returnToHome();
}

//start tictactoe extra functions
// TODO: Computer should draw a line to indicate win
bool BoardManager::ticTacToe_checkWin(char player) {
	for(int i = 0; i < 3; i++) {
		if (ticTacToe_board[i][0] == player && ticTacToe_board[i][1] == player && ticTacToe_board[i][2] == player) {
      return true;
    }

		if (ticTacToe_board[0][i] == player && ticTacToe_board[1][i] == player && ticTacToe_board[2][i] == player) {
      return true;
    }
	}

	return (ticTacToe_board[0][0] == player && ticTacToe_board[1][1] == player && ticTacToe_board[2][2] == player) ||
         (ticTacToe_board[0][2] == player && ticTacToe_board[1][1] == player && ticTacToe_board[2][0] == player);
}

// Check if the board is full (i.e., no more moves can be made)
bool BoardManager::ticTacToe_isBoardFull() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (ticTacToe_board[i][j] == TICTACTOE_EMPTY) {
        return false;
      }
    }
  }
  return true;
}

// Computer makes a random move on the board
// TODO: Make computer complete a line if a win is available
void BoardManager::ticTacToe_computerMove() {
	randomSeed(millis());
  int row, col;
  do {
    row = random(3);
    col = random(3);
  } while (!(ticTacToe_board[row][col] == TICTACTOE_EMPTY));

  ticTacToe_board[row][col] = TICTACTOE_COMPUTER;

  if(row == 0 && col == 0){
    _myGCodeHandler->drawLine(666, 218, 695, 178);
    _myGCodeHandler->drawLine(666, 178, 695, 218);
  }
  if(row == 0 && col == 1){
    _myGCodeHandler->drawLine(737, 218, 767, 178);
    _myGCodeHandler->drawLine(737, 178, 767, 218);
  }
  if(row == 0 && col == 2){
    _myGCodeHandler->drawLine(806, 218, 836, 178);
    _myGCodeHandler->drawLine(806, 178, 836, 218);
  }
  if(row == 1 && col == 0){
    _myGCodeHandler->drawLine(666, 149, 695, 109);
    _myGCodeHandler->drawLine(666, 109, 695, 149);
  }
  if(row == 1 && col == 1){
    _myGCodeHandler->drawLine(737, 149, 767, 109);
    _myGCodeHandler->drawLine(737, 109, 767, 149);
  }
  if(row == 1 && col == 2){
    _myGCodeHandler->drawLine(806, 149, 836, 109);
    _myGCodeHandler->drawLine(806, 109, 836, 149);
  }
  if(row == 2 && col == 0){
    _myGCodeHandler->drawLine(666, 69, 695, 29);
    _myGCodeHandler->drawLine(666, 29, 695, 69);
  }
  if(row == 2 && col == 1){
    _myGCodeHandler->drawLine(738, 69, 767, 29);
    _myGCodeHandler->drawLine(738, 29, 767, 69);
  }
  if(row == 2 && col == 2){
    _myGCodeHandler->drawLine(806, 69, 836, 29);
    _myGCodeHandler->drawLine(806, 29, 836, 69);
  }
	_myGCodeHandler->sendSingleCommand("G00 X811 Y307");
}
//end tictactoe extra functions
// TODO: finalizeToDos
void BoardManager::finalizeToDos() {

}

// FIXME: Weather
bool BoardManager::getWeather() {
	WiFiClient client;
	_wifiStatus = WL_IDLE_STATUS;
	// Wifi Connection
	if (WiFi.status() == WL_NO_MODULE) {
		_consoleSerial->println("Communication with WiFi module failed!");
	
		while (true){

		}
  }

	String fv = WiFi.firmwareVersion();
	if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
		_consoleSerial->println("Please upgrade the firmware");
	}

	while(_wifiStatus != WL_CONNECTED) {
		_consoleSerial->print("Attempting to connect to SSID: ");
		_consoleSerial->println(_wifiSSID);
		_wifiStatus = WiFi.begin(_wifiSSID, _wifiPass); //subbed with variables from .h
	}

	client.stop();

	_consoleSerial->println("\nStarting connection to server...");
  
  if (client.connect("api.open-meteo.com", 80)) {
		_consoleSerial->println("connected to server");
		String request = "GET /v1/forecast?latitude=";
		request += _latitude;
		request += "&longitude=";
		request += _longitude;
		request += "&temperature_unit=fahrenheit&current_weather=true HTTP/1.1";
		_consoleSerial->println(request);
		client.println(request);
		client.println("Host: api.open-meteo.com");
		client.println("Connection: close");
		client.println();
	} else {
		return false;
  	_consoleSerial->println("connection failed");
  }

	uint32_t received_data_num = 0;
	uint32_t data_num = 0;
	bool jsonDetected = false;
	char data[500];

	_consoleSerial->println("reading response...");
	while (client.available() && data_num < 500) {
		char c = client.read();
		_consoleSerial->print(c);
		if ('{' == c) {
			jsonDetected = true;
		}
		if (jsonDetected) {
			data[data_num++] = c;
		}
	}

	// TODO: Would like more weather information to display
	if (jsonDetected) {
		myObject = JSON.parse(data);
		_consoleSerial->print("Temperature F: ");
		if (myObject.hasOwnProperty("current_weather")) {
			temperature = (double)myObject["current_weather"]["temperature"];
		}
	}
}

// TODO: Make this better
void BoardManager::drawWeather() {
  _myGCodeHandler->setCursor(639.149, 473.776);
  _myGCodeHandler->setFontScale(0.8);
  _myGCodeHandler->setTextConstraints(639.149,410,853.38,473.776);
	String toWrite = "Weather: ";
	toWrite += temperature;
	toWrite += "F";
	_myGCodeHandler->write(toWrite, WRAP_WRAP, true);
}

// TODO: Quote Formatting
void BoardManager::drawQuote() {
	String quotes[31] = {
    "A winner is just a loser who tried one more time. George M. Moore, Jr.", 
    "Fall seven times, stand up eight. Japanese proverb", 
    "You miss 100 percent of the shots you do not take. Wayne Gretzky", 
    "A person who never made a mistake never tried anything new. Albert Einstein", 
    "Every strike brings me closer to the next home run. Babe Ruth", 
    "I have not failed. I have just found 10,000 ways that will not work. Thomas Edison",
    "Be the change you wish to see in the world. Gandhi",
    "Believe you can and you're halfway there. Theodore Roosevelt",
    "In the middle of difficulty lies opportunity. Albert Einstein",
    "Do one thing every day that scares you. Eleanor Roosevelt",
    "Success is not final, failure is not fatal: It is the courage to continue that counts. Winston Churchill",
    "The only way to do great work is to love what you do. Steve Jobs",
    "Happiness is not something ready-made. It comes from your own actions. Dalai Lama",
    "Dream big and dare to fail. Norman Vaughan",
    "The best way to predict the future is to invent it. Alan Kay",
  	"Life is 10 percent what happens to us and 90 percent how we react to it. Charles R. Swindoll",
    "The future belongs to those who believe in the beauty of their dreams. Eleanor Roosevelt",
    "Don't count the days, make the days count. Muhammad Ali",
    "Success is walking from failure to failure with no loss of enthusiasm. Winston Churchill",
    "You must be the change you wish to see in the world. Mahatma Gandhi",
    "Opportunities don't happen. You create them. Chris Grosser",
    "Do what you can with all you have, wherever you are. Theodore Roosevelt",
    "Your attitude, not your aptitude, will determine your altitude. Zig Ziglar",
    "The only limit to our realization of tomorrow will be our doubts of today. Franklin D. Roosevelt",
    "Don't watch the clock; do what it does. Keep going. Sam Levenson",
    "Problems are not stop signs; they are guidelines. Robert H. Schuller",
    "It does not matter how slowly you go as long as you do not stop. Confucius",
    "I attribute my success to this: I never gave or took any excuse. Florence Nightingale",
    "The best revenge is massive success. Frank Sinatra",
    "The journey of a thousand miles begins with one step. Lao Tzu",
    "Nothing is impossible, the word itself says 'I'm possible'! Audrey Hepburn"
  };

	String currentQuote;
	randomSeed(millis());
	int index = random(31);

	currentQuote = quotes[index];
	
	_myGCodeHandler->setFontScale(0.7);
	_myGCodeHandler->setTextConstraints(10,0,630, 120);
	_myGCodeHandler->setCursor(10, 70);
	_myGCodeHandler->write(currentQuote, WRAP_WRAP, true);
}

// TODO: Mood tracking revamp
void BoardManager::drawMorningMoodQs() {
  ToDoListItem moods[9] = {{"Grateful",""},{"Energetic",""},{"Peaceful",""},{"Stressed",""},{"Anxious",""},{"Okay",""},{"Sad",""},{"Angry",""},{"Content",""}};

  drawListSection(1, LIST_RIGHT, 9, "Morning Mood", moods, true, false);
}

// TODO: Mood tracking revamp
void BoardManager::drawEveningMoodQs() {
  ToDoListItem moods[9] = {{"Grateful",""},{"Energetic",""},{"Peaceful",""},{"Stressed",""},{"Anxious",""},{"Okay",""},{"Sad",""},{"Angry",""},{"Content",""}};

  drawListSection(1, LIST_RIGHT, 9, "Evening Mood", moods, true, false);
}

// TODO: Rewards
void BoardManager::updateRewards() {
	// graphic1 = (750, 400);
	// grpahic2 = (647, 393);
	// graphic3 = (627, 312);
	// graphic4 = (702, 239);
	// graphic5 = (777, 307);

	//graphics will be on SD card
	//write function to get txt file with gcode of graphic
	//draw one graphic when 20% of TO DOs are sensed by sensor

}

// TODO: Better documentation
// TODO: Could this be less of a shitshow?
void BoardManager::updateConfigFromSD() {
	File myFile;
	String input;
	char next;

	_consoleSerial->print("Initializing SD card...");
	digitalWrite(10, HIGH);

	if (!SD.begin(10)) {
		_consoleSerial->println("initialization failed!");

		_displayError("error: SD card failure", 0);
		_displayError("restart board...", 1);
		_myGCodeHandler->returnToHome();
		while(true);
	}
	_consoleSerial->println("initialization success!");

	// go through all the files and update class variables
	myFile = SD.open("name.txt");
	// expects file format one line: "firstname:lastname"
	if (myFile) {
		input = "";
		while (myFile.available()) {
			next = myFile.read();
			if (next == ':') break;
			input.concat(next);
		}
		input.toCharArray(_userFirstName, 20);

		input = "";
		while (myFile.available()) {
			next = myFile.read();
			if (next == '\n') break;
			input.concat(next);
		}

		input.toCharArray(_userLastName, 20);

		myFile.close();
	} else {
		_consoleSerial->println("error opening name.txt!");
	}

	myFile = SD.open("theme.txt");
	if (myFile) {
		_consoleSerial->println("theme.txt:");
		if (myFile.available()) {
			next = myFile.read();
			_theme = next - '0';
			_consoleSerial->print("Theme: ");
			_consoleSerial->println(_theme);
		}
		myFile.close();
	} else {
		_consoleSerial->println("error opening theme.txt!");
	}

	myFile = SD.open("wifi.txt");
	if (myFile) {
		input = "";
		while (myFile.available()) {
			next = myFile.read();
			if (next == '\n') break;
			input.concat(next);
		}

		bool hasSSID = input != "";

		input.toCharArray(_wifiSSID, 30);

		input = "";
		while (myFile.available()) {
			next = myFile.read();
			if (next == '\n') break;
			input.concat(next);
		}

		_hasWiFiInfo = hasSSID && input != "";

		input.toCharArray(_wifiPass, 30);

		myFile.close();
	} else {
		_consoleSerial->println("error opening wifi.txt!");
	}

	myFile = SD.open("features.txt");
	if (myFile) {
		input = "";
		while (myFile.available()) {
			next = myFile.read();
			if (next == '\n') break;
			input.concat(next);
		}
		_numFeatures = input.toInt();

		for (int i = 0; i < 20; i++) {
			_features[i] = false;
			if (myFile.available()) {
				next = myFile.read();
				if (next == '1') {
					_features[i] = true;
				}
			}
		}
		myFile.close();
	} else {
		_consoleSerial->println("error opening features.txt!");
	}

	myFile = SD.open("moodqs.txt");
	if (myFile) {
		input = "";
		while (myFile.available()) {
			next = myFile.read();
			if (next == '\n') break;
			input.concat(next);
		}
		_numMoodQuestions = input.toInt();

		for (int i = 0; i < 10; i++) {
			_moodQuestions[i] = false;
			if (myFile.available()) {
				next = myFile.read();
				if (next == '1') {
					_moodQuestions[i] = true;
				}
			}
		}
		myFile.close();
	} else {
		_consoleSerial->println("error opening moodqs.txt!");
	}

	myFile = SD.open("timezone.txt");
	if (myFile) {
		input = "";
		while (myFile.available()) {
			next = myFile.read();
			input.concat(next);
		}

		if (input.charAt(0) == '-') {
			_timeZoneOffsetHours = -1 * input.substring(1).toInt();
		} else {
			_timeZoneOffsetHours = input.toInt();
		}
		myFile.close();
	} else {
		_consoleSerial->println("error opening timezone.txt!");
	}

	// myFile = SD.open("coords.txt", FILE_WRITE | O_TRUNC);
	// myFile.print("40.014984\n-105.270546");
	// myFile.close();

	myFile = SD.open("coords.txt");
	if(myFile) {
		input = "";
		while (myFile.available()) {
			next = myFile.read();
			if (next == '\n') break;
			input.concat(next);
		}
		input.toCharArray(_latitude, 12);

		input = "";
		while (myFile.available()) {
			next = myFile.read();
			if (next == '\n') break;
			input.concat(next);
		}
		input.toCharArray(_longitude, 12);

		myFile.close();
	} else {
		_consoleSerial->println("error opening zip.txt!");
	}

	// myFile = SD.open("todo1.txt", FILE_WRITE | O_TRUNC);
	// myFile.print("7\nBrush teeth\nMake coffee\nMake your bed\nShower\nMeditate\nWalk the frog\nFeed the dog");
	// myFile.close();

	// myFile = SD.open("todo2.txt", FILE_WRITE | O_TRUNC);
	// myFile.print("5\nCheck emails\nReach out to a friend\nDo something creative\nExercise\nClean up");
	// myFile.close();

	// myFile = SD.open("todo3.txt", FILE_WRITE | O_TRUNC);
	// myFile.print("6\nDrink tea\nTidy apartment\nCheck morning events\nTake out dog\nBrush teeth\nTake meds");
	// myFile.close();

	myFile = SD.open("todo1.txt");
	if (myFile) {
		input = "";
		while (myFile.available()) {
			next = myFile.read();
			if (next == '\n') break;
			input.concat(next);
		}
		_numMorningToDos = input.toInt();

		for (int i = 0; i < _numMorningToDos; i++) {
			input = "";
			while (myFile.available()) {
				next = myFile.read();
				if (next == '\n') break;
				input.concat(next);
			}
			input.toCharArray(_morningToDoList[i].name, 50);
		}
		myFile.close();
	} else {
		_consoleSerial->println("error opening todo1.txt!");
	}

	myFile = SD.open("todo2.txt");
	if (myFile) {
		input = "";
		while (myFile.available()) {
			next = myFile.read();
			if (next == '\n') break;
			input.concat(next);
		}
		_numDayToDos = input.toInt();

		for (int i = 0; i < _numDayToDos; i++) {
			input = "";
			while (myFile.available()) {
				next = myFile.read();
				if (next == '\n') break;
				input.concat(next);
			}
			input.toCharArray(_dayToDoList[i].name, 50);
		}
		myFile.close();
	} else {
		_consoleSerial->println("error opening todo2.txt!");
	}

	myFile = SD.open("todo3.txt");
	if (myFile) {
		input = "";
		while (myFile.available()) {
			next = myFile.read();
			if (next == '\n') break;
			input.concat(next);
		}
		_numEveningToDos = input.toInt();

		for (int i = 0; i < _numEveningToDos; i++) {
			input = "";
			while (myFile.available()) {
				next = myFile.read();
				if (next == '\n')
					break;
				input.concat(next);
			}
			input.toCharArray(_eveningToDoList[i].name, 50);
		}
		myFile.close();
	} else {
		_consoleSerial->println("error opening todo2.txt!");
	}
}

// TODO: Indicate to user upon completion
void BoardManager::openBluetoothBLE() {
	unsigned long lastBluetoothEvent = millis();
	BLEService whiteboardService("722cf000-6c3d-48ac-8180-64551d967680");
	BLEDevice central;

	BLECharacteristic dataCharacteristic("722cf001-6c3d-48ac-8180-64551d967680", BLERead | BLEWrite | BLENotify, 200, false);
	BLECharacteristic requestNameCharacteristic("722cf002-6c3d-48ac-8180-64551d967680", BLERead | BLEWrite, 50, false);
	BLEIntCharacteristic indexCharacteristic("722cf003-6c3d-48ac-8180-64551d967680", BLERead | BLEWrite);
	BLEBoolCharacteristic portalSideRequestCharacteristic("722cf004-6c3d-48ac-8180-64551d967680", BLEWrite);
	BLEBoolCharacteristic portalHasUpdateCharacteristic("722cf005-6c3d-48ac-8180-64551d967680", BLEWrite);
	BLEBoolCharacteristic successResponseCharacteristic("722cf006-6c3d-48ac-8180-64551d967680", BLENotify);

	if (!BLE.begin()) {
		_consoleSerial->println("Starting BLE failed!");
		_displayError("error: BLE failure", 0);
		_displayError("restart board...", 1);
		_myGCodeHandler->returnToHome();
		while(true);
	}

	BLE.setLocalName("WhiteboardConfig");
	BLE.setAdvertisedService(whiteboardService);

	whiteboardService.addCharacteristic(dataCharacteristic);
	whiteboardService.addCharacteristic(requestNameCharacteristic);
	whiteboardService.addCharacteristic(indexCharacteristic);
	whiteboardService.addCharacteristic(portalSideRequestCharacteristic);
	whiteboardService.addCharacteristic(portalHasUpdateCharacteristic);
	whiteboardService.addCharacteristic(successResponseCharacteristic);

	BLE.addService(whiteboardService);

	// this is where the current configurations are advertise
	dataCharacteristic.writeValue("default");

	BLE.setEventHandler(BLEConnected, bleConnectHandler);
	BLE.setEventHandler(BLEDisconnected, bleDisconnectHandler);

	BLE.advertise();

	_consoleSerial->println("Begin Bluetooth communication...");

	unsigned long lastCheck = 0;
	char *requestType;
	char *updateType;
	String tempString = "";
	char temp[20];
	while (true) {
		updateButtonStates();
		if(_buttonStates[BUTTON_INDEX[6]] && (unsigned long)(millis() - lastBluetoothEvent) >= 3000) {
			_lastButtonPressTimes[6] = millis();
			break;
		}
		BLE.poll();

		unsigned long t = millis();

		if (central.connected()) {
			// this is where most of the reading of changes happens.
			if (portalSideRequestCharacteristic.written()) {
				lastBluetoothEvent = millis();
				if (portalSideRequestCharacteristic.value()) {
					requestType = (char *)requestNameCharacteristic.value();
					if (strcmp(requestType, "firstName") == 0) {
						dataCharacteristic.writeValue(_userFirstName);
					} else if (strcmp(requestType, "lastName") == 0) {
						dataCharacteristic.writeValue(_userLastName);
					} else if (strcmp(requestType, "theme") == 0) {
						dataCharacteristic.writeValue(itoa(_theme, temp, 10));
					} else if (strcmp(requestType, "features") == 0) {
						tempString = "";
						for (int i = 0; i < _numFeatures; i++) {
							tempString.concat(_features[i]);
						}
						tempString.toCharArray(temp, 20);
						dataCharacteristic.writeValue(temp);
					} else if (strcmp(requestType, "numFeatures") == 0) {
						dataCharacteristic.writeValue(itoa(_numFeatures, temp, 10));
					} else if (strcmp(requestType, "moodQuestions") == 0) {
						tempString = "";
						for (int i = 0; i < _numMoodQuestions; i++) {
							tempString.concat(_moodQuestions[i]);
						}
						tempString.toCharArray(temp, 20);
						dataCharacteristic.writeValue(temp);
					} else if (strcmp(requestType, "numMoodQuestions") == 0) {
						dataCharacteristic.writeValue(itoa(_numMoodQuestions, temp, 10));
					} else if (strcmp(requestType, "wifiSSID") == 0) {
						dataCharacteristic.writeValue(_wifiSSID);
					} else if (strcmp(requestType, "wifiPass") == 0) {
						dataCharacteristic.writeValue(_wifiPass);
					} else if (strcmp(requestType, "timeZone") == 0) {
						dataCharacteristic.writeValue(itoa(_timeZoneOffsetHours, temp, 10));
					} else if (strcmp(requestType, "latitude") == 0) {
						dataCharacteristic.writeValue(_latitude);
					} else if (strcmp(requestType, "longitude") == 0) {
						dataCharacteristic.writeValue(_longitude);
					} else if (strcmp(requestType, "numMorningToDos") == 0) {
						dataCharacteristic.writeValue(itoa(_numMorningToDos, temp, 10));
					} else if (strcmp(requestType, "morningToDo") == 0) {
						dataCharacteristic.writeValue(_morningToDoList[indexCharacteristic.value()].name);
					} else if (strcmp(requestType, "numDaytimeToDos") == 0) {
						dataCharacteristic.writeValue(itoa(_numDayToDos, temp, 10));
					} else if (strcmp(requestType, "daytimeToDo") == 0) {
						dataCharacteristic.writeValue(_dayToDoList[indexCharacteristic.value()].name);
					} else if (strcmp(requestType, "numEveningToDos") == 0) {
						dataCharacteristic.writeValue(itoa(_numEveningToDos, temp, 10));
					} else if (strcmp(requestType, "eveningToDo") == 0) {
						dataCharacteristic.writeValue(_eveningToDoList[indexCharacteristic.value()].name);
					}
					portalSideRequestCharacteristic.writeValue(false);
				}
			}

			if (portalHasUpdateCharacteristic.written()) {
				lastBluetoothEvent = millis();
				if (portalHasUpdateCharacteristic.value()) {
					File myFile;
					updateType = (char *)requestNameCharacteristic.value();
					tempString = (char *)dataCharacteristic.value();

					if (strcmp(updateType, "theme") == 0) {
						_consoleSerial->println(tempString);
						_theme = tempString.toInt();
						myFile = SD.open("theme.txt", FILE_WRITE | O_TRUNC);
						myFile.print(tempString);
						myFile.close();
					} else if(strcmp(updateType, "firstName") == 0) {
						_consoleSerial->println(tempString);
						tempString.toCharArray(_userFirstName, 20);
						myFile = SD.open("name.txt", FILE_WRITE | O_TRUNC);
						myFile.print(_userFirstName);
						myFile.print(':');
						myFile.print(_userLastName);
						myFile.close();
					} else if(strcmp(updateType, "lastName") == 0) {
						_consoleSerial->println(tempString);
						tempString.toCharArray(_userLastName, 20);
						myFile = SD.open("name.txt", FILE_WRITE | O_TRUNC);
						myFile.print(_userFirstName);
						myFile.print(':');
						myFile.print(_userLastName);
						myFile.close();
					} else if(strcmp(updateType, "features") == 0) {
						_consoleSerial->println(tempString);
						for(int i = 0; i < _numFeatures; i++) {
							_features[i] = (tempString.charAt(i) == '1');
						}
						myFile = SD.open("features.txt", FILE_WRITE | O_TRUNC);
						myFile.println(_numFeatures);
						myFile.print(tempString);
						myFile.close();
					} else if(strcmp(updateType, "numFeatures") == 0) {
						
					} else if(strcmp(updateType, "moodQuestions") == 0) {
						
					} else if(strcmp(updateType, "numMoodQuestions") == 0) {
						
					} else if(strcmp(updateType, "wifiSSID") == 0) {
						_consoleSerial->println(tempString);
						tempString.toCharArray(_wifiSSID, 30);
						myFile = SD.open("wifi.txt", FILE_WRITE | O_TRUNC);
						myFile.print(_wifiSSID);
						myFile.print('\n');
						myFile.print(_wifiPass);
						myFile.close();
					} else if(strcmp(updateType, "wifiPass") == 0) {
						_consoleSerial->println(tempString);
						tempString.toCharArray(_wifiPass, 30);
						myFile = SD.open("wifi.txt", FILE_WRITE | O_TRUNC);
						myFile.print(_wifiSSID);
						myFile.print('\n');
						myFile.print(_wifiPass);
						myFile.close();
					} else if(strcmp(updateType, "timeZone") == 0) {
						_consoleSerial->println(tempString);
						_timeZoneOffsetHours = tempString.toInt();
						myFile = SD.open("timezone.txt", FILE_WRITE | O_TRUNC);
						myFile.print(_timeZoneOffsetHours);
						myFile.close();
					} else if(strcmp(updateType, "latitude") == 0) {
						_consoleSerial->println(tempString);
						tempString.toCharArray(_latitude, 12);
						myFile = SD.open("coords.txt", FILE_WRITE | O_TRUNC);
						myFile.print(_latitude);
						myFile.print("\n");
						myFile.print(_longitude);
						myFile.close();
					} else if(strcmp(updateType, "longitude") == 0) {
						_consoleSerial->println(tempString);
						tempString.toCharArray(_longitude, 12);
						myFile = SD.open("coords.txt", FILE_WRITE | O_TRUNC);
						myFile.print(_latitude);
						myFile.print("\n");
						myFile.print(_longitude);
						myFile.close();
					} else if(strcmp(updateType, "numMorningToDos") == 0) {
						_consoleSerial->println(tempString);
						_numMorningToDos = tempString.toInt();
					} else if(strcmp(updateType, "morningToDo") == 0) {
						_consoleSerial->println(tempString);
						tempString.toCharArray(_morningToDoList[indexCharacteristic.value()].name, 50);
					} else if(strcmp(updateType, "saveMorningToDos") == 0) {
						_consoleSerial->println("Saving morning to dos...");
						myFile = SD.open("todo1.txt", FILE_WRITE | O_TRUNC);
						myFile.print(_numMorningToDos);
						myFile.print('\n');
						for(int i = 0; i < _numMorningToDos; i++) {
							myFile.print(_morningToDoList[i].name);
							if(i != _numMorningToDos - 1) {
								myFile.print('\n');
							}
						}
						myFile.close();
					} else if(strcmp(updateType, "numDaytimeToDos") == 0) {
						_consoleSerial->println(tempString);
						_numDayToDos = tempString.toInt();
					} else if(strcmp(updateType, "daytimeToDo") == 0) {
						_consoleSerial->println(tempString);
						tempString.toCharArray(_dayToDoList[indexCharacteristic.value()].name, 50);
					} else if(strcmp(updateType, "saveDaytimeToDos") == 0) {
						_consoleSerial->println("Saving daytime to dos...");
						myFile = SD.open("todo2.txt", FILE_WRITE | O_TRUNC);
						myFile.print(_numDayToDos);
						myFile.print('\n');
						for(int i = 0; i < _numDayToDos; i++) {
							myFile.print(_dayToDoList[i].name);
							if(i != _numDayToDos - 1) {
								myFile.print('\n');
							}
						}
						myFile.close();
					} else if(strcmp(updateType, "numEveningToDos") == 0) {
						_consoleSerial->println(tempString);
						_numEveningToDos = tempString.toInt();
					} else if(strcmp(updateType, "eveningToDo") == 0) {
						_consoleSerial->println(tempString);
						tempString.toCharArray(_eveningToDoList[indexCharacteristic.value()].name, 50);
					} else if(strcmp(updateType, "saveEveningToDos") == 0) {
						_consoleSerial->println("Saving evening to dos...");
						myFile = SD.open("todo3.txt", FILE_WRITE | O_TRUNC);
						myFile.print(_numEveningToDos);
						myFile.print('\n');
						for(int i = 0; i < _numEveningToDos; i++) {
							myFile.print(_eveningToDoList[i].name);
							if(i != _numEveningToDos - 1) {
								myFile.print('\n');
							}
						}
						myFile.close();
					}

					portalHasUpdateCharacteristic.writeValue(false);
				}
			}
		} else if ((unsigned long)(t - lastCheck) > 100) {
			central = BLE.central();
			lastCheck = t;
		}
	}

	BLE.end();
}

void bleConnectHandler(BLEDevice central) {
	Serial.print("Connected to: ");
	Serial.println(central.address());
}

void bleDisconnectHandler(BLEDevice central) {
	Serial.print("Disconnected from: ");
	Serial.println(central.address());
}

bool BoardManager::_connectToWifi() {
	// check for the WiFi module:
	if (WiFi.status() == WL_NO_MODULE) {
		_consoleSerial->println("Communication with WiFi module failed!");
		return false;
	}

	String fv = WiFi.firmwareVersion();
	if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
		_consoleSerial->println("Please upgrade the firmware!");
	}

	// attempt to connect to WiFi network:
	int attemptCount = 0;
	while (_wifiStatus != WL_CONNECTED && attemptCount <= 10) {
		updateButtonStates();
		if(_buttonStates[BUTTON_INDEX[6]]) {
			_consoleSerial->println("Opening bluetooth override.");
			openBluetoothBLE();
		}

		_consoleSerial->print("Attempting to connect to SSID: ");
		_consoleSerial->println(_wifiSSID);
		_wifiStatus = WiFi.begin(_wifiSSID, _wifiPass);
		attemptCount++;
		delay(500);
	}

	if(attemptCount > 10) {
		_displayError("error: wifi failure", 0);
		_displayError("some features will not work", 1);
		_myGCodeHandler->returnToHome();
		return false;
	}

	_consoleSerial->println("Connected to WiFi!");
	_printWifiStatus();
}

void BoardManager::_printWifiStatus() {
	// print the SSID of the network you're attached to:
	_consoleSerial->print("SSID: ");
	_consoleSerial->println(WiFi.SSID());

	// print your board's IP address:
	IPAddress ip = WiFi.localIP();
	_consoleSerial->print("IP Address: ");
	_consoleSerial->println(ip);

	// print the received signal strength:
	long rssi = WiFi.RSSI();
	_consoleSerial->print("signal strength (RSSI):");
	_consoleSerial->print(rssi);
	_consoleSerial->println(" dBm");
}

// TODO: Interrupt with button press?
void BoardManager::NTP() {
	auto unixTime = _timeClient->getEpochTime() + (_timeZoneOffsetHours * 3600);
	RTCTime timeToSet = RTCTime(unixTime);
	RTC.setTime(timeToSet);
	_consoleSerial->print("Time retrieved: ");
	_consoleSerial->println(timeToSet.toString());
	lastTimeUpdate = millis();
}

void BoardManager::drawListSection(double rowStart, int leftOrRight, int numItems, char *listName, ToDoListItem *itemList, bool hasCheckboxes, bool hasLeftLabel) {
	if(numItems > 12 - (rowStart - 1)) {
		numItems = 12 - (rowStart - 1);
	}
	
	double startX = leftOrRight == LIST_LEFT ? LAYOUT_TODO_LEFT_X_START : LAYOUT_TODO_RIGHT_X_START;
	double leftLineX = startX;
	double sensorX = leftOrRight == LIST_LEFT ? LAYOUT_LEFT_SENSOR_X : LAYOUT_RIGHT_SENSOR_X;
	double rightLineX = sensorX - LAYOUT_TODO_LINE_HEIGHT / 2.0;
	double endX = sensorX + LAYOUT_TODO_LINE_HEIGHT / 2.0;
	if (hasLeftLabel) {
		leftLineX += LAYOUT_TODO_LEFT_LABEL_SPACE;
	}
	double startY = LAYOUT_FIRST_SENSOR_Y - ((rowStart - 1.5) * LAYOUT_TODO_LINE_HEIGHT);

	double endY = startY - ((numItems + 1) * LAYOUT_TODO_LINE_HEIGHT);

	_myGCodeHandler->drawLine(leftLineX, startY, leftLineX, endY);
	for (int i = 1; i <= numItems + 1; i++) {
		_myGCodeHandler->drawLine(startX, startY - (i * LAYOUT_TODO_LINE_HEIGHT), endX, startY - (i * LAYOUT_TODO_LINE_HEIGHT));
	}

	if (hasCheckboxes) {
		_myGCodeHandler->drawLine(rightLineX, startY, rightLineX, endY);
	}
	_myGCodeHandler->setFontScale(LAYOUT_TODO_ITEM_FONT_SCALE);
	_myGCodeHandler->setTextConstraints(leftLineX, startY, rightLineX, endY);

	double cursorOffsetX = LAYOUT_TODO_CURSOR_OFFSET_X;
	double cursorOffsetY = (LAYOUT_TODO_LINE_HEIGHT - LETTER_CAP_HEIGHT * _myGCodeHandler->getFontScale()) / 2.0;

	_myGCodeHandler->setCursor(leftLineX + cursorOffsetX, startY - LAYOUT_TODO_LINE_HEIGHT + cursorOffsetY);

	_myGCodeHandler->write(listName, WRAP_TRUNCATE, true);

	for(int i = 0; i < numItems; i++) {
		_myGCodeHandler->setCursor(leftLineX + cursorOffsetX, startY - ((i + 2) * LAYOUT_TODO_LINE_HEIGHT) + cursorOffsetY);
		_myGCodeHandler->write(itemList[i].name, WRAP_TRUNCATE, true);
	}

	if (hasLeftLabel) {
		_myGCodeHandler->setFontScale(LAYOUT_TODO_LABEL_FONT_SCALE);
		_myGCodeHandler->setTextConstraints(startX, startY, startX + LAYOUT_TODO_LEFT_LABEL_SPACE, endY);

		cursorOffsetY = (LAYOUT_TODO_LINE_HEIGHT - LETTER_CAP_HEIGHT * _myGCodeHandler->getFontScale()) / 2.0;

		for (int i = 0; i < numItems; i++) {
			_myGCodeHandler->setCursor(startX, startY - ((i + 2) * LAYOUT_TODO_LINE_HEIGHT) + cursorOffsetY);
			_myGCodeHandler->write(itemList[i].label, WRAP_TRUNCATE, true);
		}
	}
}

void BoardManager::_displayError(const char* errorMessage, int lineNumber) {
	_myGCodeHandler->setCursor(LAYOUT_ERROR_START_X, LAYOUT_ERROR_START_Y - lineNumber * LINE_HEIGHT * LAYOUT_ERROR_FONT_SCALE);
	_myGCodeHandler->setFontScale(LAYOUT_ERROR_FONT_SCALE);
	_myGCodeHandler->write(errorMessage, WRAP_WRAP, false);
}

//reads all button states in just over 1ms
void BoardManager::updateButtonStates() {
	for(int i = 0; i < 16; i++) {
    digitalWrite(PIN_MULTIPLEXER_S0, bitRead(i, 0));
    digitalWrite(PIN_MULTIPLEXER_S1, bitRead(i, 1));
    digitalWrite(PIN_MULTIPLEXER_S2, bitRead(i, 2));
    digitalWrite(PIN_MULTIPLEXER_S3, bitRead(i, 3));
    delayMicroseconds(63);

		_buttonStates[i] = digitalRead(PIN_MULTIPLEXER_SIGNAL_BUTTON);
  }
}

//reads all hall effect sensor values in just over 1ms
void BoardManager::updateHallEffectStates() {
	for(int i = 0; i < 16; i++) {
    digitalWrite(PIN_MULTIPLEXER_S0, bitRead(i, 0));
    digitalWrite(PIN_MULTIPLEXER_S1, bitRead(i, 1));
    digitalWrite(PIN_MULTIPLEXER_S2, bitRead(i, 2));
    digitalWrite(PIN_MULTIPLEXER_S3, bitRead(i, 3));
    delayMicroseconds(63);

		_hallSensorValues[i] = analogRead(PIN_MULTIPLEXER_SIGNAL_HALL1);
		_hallSensorValues[i + 16] = analogRead(PIN_MULTIPLEXER_SIGNAL_HALL2);
		_hallSensorValues[i + 32] = analogRead(PIN_MULTIPLEXER_SIGNAL_HALL3);
		_hallSensorValues[i + 48] = analogRead(PIN_MULTIPLEXER_SIGNAL_HALL4);
  }

	for(int i = 0; i < 64; i++) {
		_hallSensorStates[i] = _hallSensorValues[i] > 560 || _hallSensorValues[i] < 440;
	}
}
