#include "BoardManager.h"

void bleConnectHandler(BLEDevice central);
void bleDisconnectHandler(BLEDevice central);

BoardManager::BoardManager(Stream &consoleSerial, GCodeHandler &myGCodeHandler, NTPClient &timeClient, RTCTime &currentTime, bool *buttonStates, int *hallSensorValues, bool *hallSensorStates) {
	_consoleSerial = &consoleSerial;
	_myGCodeHandler = &myGCodeHandler;
	_timeClient = &timeClient;
	_currentTime = &currentTime;
	_buttonStates = buttonStates;
	_hallSensorValues = hallSensorValues;
	_hallSensorStates = hallSensorStates;
}

void BoardManager::initialize() {
	_lastUserInteractionTime = millis() - USER_INTERACTION_WAIT_COOLDOWN;
	_myGCodeHandler->initialize();
	updateFromConfig();

	openBluetoothBLE();
	if(_hasWiFiInfo) {
		_connectToWifi();
	} else {
		_displayError("Wifi credentials required!", 0);
		_displayError("Please configure with bluetooth.", 1);
	}
	RTC.begin();
	_consoleSerial->println("\nStarting connection to server...");
	_timeClient->begin();
	_timeClient->update();

	// Get the current time from NTP
	NTP();

	RTC.getTime(*_currentTime);
	_currentDay = _currentTime->getUnixTime() / 86400UL;
}

void BoardManager::update() {
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

void BoardManager::morningUpdate() {
	//check previous to dos

	//print good morning
	String text = "Good morning, ";
	text += _userFirstName;
	text += "!";
	_myGCodeHandler->setCursor(TITLE_START_X, TITLE_START_Y);
	_myGCodeHandler->setFontScale(TITLE_FONT_SCALE);
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
	_myGCodeHandler->setCursor(SUBTITLE_START_X, SUBTITLE_START_Y);
	_myGCodeHandler->setFontScale(SUBTITLE_FONT_SCALE);
	_myGCodeHandler->write(text, WRAP_TRUNCATE, false);
	//print todos
	drawListSection(TODO_Y_START, TODO_LEFT_X_START, _numMorningToDos, "Morning To-Do", _morningToDoList, true, false);
	//print events?
	//print weather
	//print quote
	//print sleep survey
}

void BoardManager::forceMorningUpdate() {
	_lastUserInteractionTime = millis();
	morningUpdate();
}

void BoardManager::daytimeUpdate() {
}

void BoardManager::forceDaytimeUpdate() {
	_lastUserInteractionTime = millis();
	daytimeUpdate();
}

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

}

void BoardManager::finalizeToDos() {
	
}

void BoardManager::updateFromConfig() {
	File myFile;
	String input;
	char next;

	_consoleSerial->print("Initializing SD card...");
	digitalWrite(10, HIGH);

	if (!SD.begin(10)) {
		_consoleSerial->println("initialization failed!");
		_displayError("error: SD card failure", 0);
		_displayError("restart board...", 1);
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
		if(_buttonStates[BUTTON_INDEX[6]] && (unsigned long)(millis() - lastBluetoothEvent) >= 3000) break;
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
		_consoleSerial->print("Attempting to connect to SSID: ");
		_consoleSerial->println(_wifiSSID);
		_wifiStatus = WiFi.begin(_wifiSSID, _wifiPass);
		attemptCount++;
		delay(1000);
	}

	if(attemptCount > 10) {
		_displayError("error: wifi failure", 0);
		_displayError("some features will not work", 1);
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

void BoardManager::NTP() {
	auto unixTime = _timeClient->getEpochTime() + (_timeZoneOffsetHours * 3600);
	RTCTime timeToSet = RTCTime(unixTime);
	RTC.setTime(timeToSet);
	lastTimeUpdate = millis();
}

void BoardManager::drawListSection(double startY, double startX, int numItems, char *listName, ToDoListItem *itemList, bool hasCheckboxes, bool hasLeftLabel) {
	double leftLineX = startX;
	double endX = startX + TODO_ITEM_WIDTH;
	if (hasLeftLabel) {
		leftLineX += TODO_LEFT_LABEL_SPACE;
	}

	double endY = startY - ((numItems + 1) * TODO_LINE_HEIGHT);

	_myGCodeHandler->drawLine(leftLineX, startY, leftLineX, endY);
	for (int i = 1; i <= numItems; i++) {
		_myGCodeHandler->drawLine(startX, startY - (i * TODO_LINE_HEIGHT), startX + TODO_ITEM_WIDTH, startY - (i * TODO_LINE_HEIGHT));
	}

	if (hasCheckboxes) {
		endX -= TODO_CHECKBOX_SPACE;
		_myGCodeHandler->drawLine(startX + TODO_ITEM_WIDTH - TODO_CHECKBOX_SPACE, startY, startX + TODO_ITEM_WIDTH - TODO_CHECKBOX_SPACE, endY);
	}

	_myGCodeHandler->setFontScale(TODO_ITEM_FONT_SCALE);
	_myGCodeHandler->setTextConstraints(startX, startY, endX, endY);

	double cursorOffsetX = TODO_CURSOR_OFFSET_X;
	double cursorOffsetY = (TODO_LINE_HEIGHT - LETTER_CAP_HEIGHT * _myGCodeHandler->getFontScale()) / 2.0;

	_myGCodeHandler->setCursor(leftLineX + cursorOffsetX, startY - TODO_LINE_HEIGHT + cursorOffsetY);

	_myGCodeHandler->write(listName, WRAP_TRUNCATE, true);
	_consoleSerial->println("d");

	for (int i = 0; i < numItems; i++) {
		_myGCodeHandler->setCursor(leftLineX + cursorOffsetX, startY - ((i + 2) * TODO_LINE_HEIGHT) + cursorOffsetY);
		_myGCodeHandler->write(itemList[i].name, WRAP_TRUNCATE, true);
	}

	if (hasLeftLabel) {
		_myGCodeHandler->setFontScale(TODO_LABEL_FONT_SCALE);
		_myGCodeHandler->setTextConstraints(startX, startY, startX + TODO_LEFT_LABEL_SPACE, endY);

		cursorOffsetY = (TODO_LINE_HEIGHT - LETTER_CAP_HEIGHT * _myGCodeHandler->getFontScale()) / 2.0;

		for (int i = 0; i < numItems; i++) {
			_myGCodeHandler->setCursor(startX, startY - ((i + 2) * TODO_LINE_HEIGHT) + cursorOffsetY);
			_myGCodeHandler->write(itemList[i].label, WRAP_TRUNCATE, true);
		}
	}
}

void BoardManager::_displayError(const char* errorMessage, int lineNumber) {
	_myGCodeHandler->setCursor(ERROR_START_X, ERROR_START_Y - lineNumber * LINE_HEIGHT * ERROR_FONT_SCALE);
	_myGCodeHandler->setFontScale(ERROR_FONT_SCALE);
	_myGCodeHandler->write(errorMessage, WRAP_ELLIPSES, false);
	_myGCodeHandler->returnToHome();
}

//reads all button states in just over 1ms
void BoardManager::updateButtonStates() {
	for(int i = 0; i < 16; i++) {
    digitalWrite(MULTI_SELECT0, bitRead(i, 0));
    digitalWrite(MULTI_SELECT1, bitRead(i, 1));
    digitalWrite(MULTI_SELECT2, bitRead(i, 2));
    digitalWrite(MULTI_SELECT3, bitRead(i, 3));
    delayMicroseconds(63);

		_buttonStates[i] = digitalRead(SIGNAL_BUTTON_MULTI);
  }
}

//reads all hall effect sensor values in just over 1ms
void BoardManager::updateHallEffectStates() {
	for(int i = 0; i < 16; i++) {
    digitalWrite(MULTI_SELECT0, bitRead(i, 0));
    digitalWrite(MULTI_SELECT1, bitRead(i, 1));
    digitalWrite(MULTI_SELECT2, bitRead(i, 2));
    digitalWrite(MULTI_SELECT3, bitRead(i, 3));
    delayMicroseconds(63);

		_hallSensorValues[i] = analogRead(SIGNAL_HALL_MULTI1);
		_hallSensorValues[i + 16] = analogRead(SIGNAL_HALL_MULTI2);
		_hallSensorValues[i + 32] = analogRead(SIGNAL_HALL_MULTI3);
		_hallSensorValues[i + 48] = analogRead(SIGNAL_HALL_MULTI4);
  }

	for(int i = 0; i < 64; i++) {
		_hallSensorStates[i] = _hallSensorValues[i] > 560 || _hallSensorValues[i] < 440;
	}
}