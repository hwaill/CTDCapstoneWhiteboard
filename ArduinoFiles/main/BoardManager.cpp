#include "BoardManager.h"

void bleConnectHandler(BLEDevice central);
void bleDisconnectHandler(BLEDevice central);

BoardManager::BoardManager(Stream &consoleSerial, GCodeHandler &myGCodeHandler, NTPClient &timeClient, RTCTime &currentTime, bool *buttonStates, int *hallSensorValues) {
	_consoleSerial = &consoleSerial;
	_myGCodeHandler = &myGCodeHandler;
	_timeClient = &timeClient;
	_currentTime = &currentTime;
	_buttonStates = buttonStates;
	_hallSensorValues = hallSensorValues;
}

void BoardManager::initialize() {
	//_myGCodeHandler->initialize();
	updateFromConfig();
	//_checkForWifiInfo();
	if(_hasWiFiInfo) {
		//_connectToWifi();
	}
	RTC.begin();
	_consoleSerial->println("\nStarting connection to server...");
	_timeClient->begin();
	_timeClient->update();

	// Get the current time from NTP
  NTP();
}

void BoardManager::updateFromConfig() {
	File myFile;
	String input;
	char next;

	_consoleSerial->print("Initializing SD card...");
	digitalWrite(10, HIGH);

	if(!SD.begin(10)) {
		_consoleSerial->println("initialization failed!");
		return;
	}
	_consoleSerial->println("initialization success!");

	//go through all the files and update class variables
	myFile = SD.open("name.txt");
	//expects file format one line: "firstname:lastname"
	if(myFile) {
		input = "";
		_consoleSerial->println("name.txt:");
		while(myFile.available()) {
			next = myFile.read();
			if(next == ':') break;
			input.concat(next);
		}
		input.toCharArray(_userFirstName, 20);
		_consoleSerial->print("First name: ");
		_consoleSerial->println(_userFirstName);

		input = "";
		while(myFile.available()) {
			next = myFile.read();
			if(next == '\n') break;
			input.concat(next);
		}

		input.toCharArray(_userLastName, 20);
		_consoleSerial->print("Last name: ");
		_consoleSerial->println(_userLastName);

		myFile.close();
	} else {
		_consoleSerial->println("error opening name.txt!");
	}

	myFile = SD.open("theme.txt");
	if(myFile) {
		_consoleSerial->println("theme.txt:");
		if(myFile.available()) {
			next = myFile.read();
			_theme = next - '0';
			_consoleSerial->print("Theme: ");
			_consoleSerial->println(_theme);
		}
		myFile.close();
	} else {
		_consoleSerial->println("error opening theme.txt!");
	}
}

void BoardManager::openBluetoothBLE() {
	bool stayConnected = true;

	BLEService whiteboardService("722cf000-6c3d-48ac-8180-64551d967680");
	BLEDevice central;

	BLECharacteristic toDoCharacteristic("722cf001-6c3d-48ac-8180-64551d967680", BLERead | BLEWrite | BLENotify, "0000010000");
	BLEBoolCharacteristic ledCharacteristic("722cf002-6c3d-48ac-8180-64551d967680", BLERead | BLEWrite);
	

	//these will be marked true by the web portal when updates are ready to be read.
	BLEBoolCharacteristic updateThemeCharacteristic("722cz000-6c3d-48ac-8180-64551d967680", BLEWrite);
	BLEBoolCharacteristic updateTodosCharacteristic("722cz001-6c3d-48ac-8180-64551d967680", BLEWrite);
	BLEBoolCharacteristic updateFeaturesCharacteristic("722cz002-6c3d-48ac-8180-64551d967680", BLEWrite);
	BLEBoolCharacteristic updateNameCharacteristic("722cz003-6c3d-48ac-8180-64551d967680", BLEWrite);
	BLEBoolCharacteristic updateTimeZoneCharacteristic("722cz004-6c3d-48ac-8180-64551d967680", BLEWrite);
	BLEBoolCharacteristic disconnectBluetoothCharacteristic("722cz005-6c3d-48ac-8180-64551d967680", BLEWrite);


	if(!BLE.begin()) {
		_consoleSerial->println("Starting BLE failed!");
	}

	BLE.setLocalName("WhiteboardConfig");
	BLE.setAdvertisedService(whiteboardService);

	whiteboardService.addCharacteristic(toDoCharacteristic);
	whiteboardService.addCharacteristic(ledCharacteristic);

	BLE.addService(whiteboardService);

	//this is where the current configurations are advertised
	ledCharacteristic.setValue(0);

	BLE.setEventHandler(BLEConnected, bleConnectHandler);
	BLE.setEventHandler(BLEDisconnected, bleDisconnectHandler);

	BLE.advertise();

	_consoleSerial->println("Begin Bluetooth communication...");

	unsigned long lastCheck = 0;

	while(stayConnected) {
		BLE.poll();

		unsigned long t = millis();
		
		if(central.connected()) {
			//this is where most of the reading of changes happens.
		} else if((unsigned long)(t - lastCheck) > 100) {
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

void BoardManager::_checkForWifiInfo() {
	_wifiSSID = "iPhone";
	_wifiPass = "henryhenryhenry";
	_hasWiFiInfo = true;
}

bool BoardManager::_connectToWifi() {
	// check for the WiFi module:
	if(WiFi.status() == WL_NO_MODULE) {
		_consoleSerial->println("Communication with WiFi module failed!");
		return false;
	}

	String fv = WiFi.firmwareVersion();
	if(fv < WIFI_FIRMWARE_LATEST_VERSION) {
		_consoleSerial->println("Please upgrade the firmware!");
	}

	// attempt to connect to WiFi network:
	while(_wifiStatus != WL_CONNECTED) {
		_consoleSerial->print("Attempting to connect to SSID: ");
		_consoleSerial->println(_wifiSSID);
		_wifiStatus = WiFi.begin(_wifiSSID, _wifiPass);
		delay(1000);
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

void BoardManager::drawListSection(double startY, double startX, int numItems, char* listName, ToDoListItem *itemList, bool hasCheckboxes, bool hasLeftLabel) {
	double leftLineX = startX;
	double endX = startX + TODO_ITEM_WIDTH;
	if(hasLeftLabel) {
		leftLineX += TODO_LEFT_LABEL_SPACE;
	}

	double endY = startY - ((numItems + 1) * TODO_LINE_HEIGHT);

	_myGCodeHandler->drawLine(leftLineX, startY, leftLineX, endY);
	for(int i = 1; i <= numItems; i++) {
		_myGCodeHandler->drawLine(startX, startY - (i * TODO_LINE_HEIGHT), startX + TODO_ITEM_WIDTH, startY - (1 * TODO_LINE_HEIGHT));
	}
	if(hasCheckboxes) {
		endX -= TODO_CHECKBOX_SPACE;
		_myGCodeHandler->drawLine(startX + TODO_ITEM_WIDTH - TODO_CHECKBOX_SPACE, startY, startX + TODO_ITEM_WIDTH - TODO_CHECKBOX_SPACE, endY);
	}

	_myGCodeHandler->setFontScale(TODO_ITEM_FONT_SCALE);
	_myGCodeHandler->setTextConstraints(startX, startY, endX, endY);

	double cursorOffsetX = TODO_CURSOR_OFFSET_X;
	double cursorOffsetY = (TODO_LINE_HEIGHT - LETTER_CAP_HEIGHT * _myGCodeHandler->getFontScale()) / 2.0;
	
	_myGCodeHandler->setCursor(leftLineX + cursorOffsetX, startY - TODO_LINE_HEIGHT + cursorOffsetY);
	_myGCodeHandler->write(listName, WRAP_TRUNCATE, true);

	for(int i = 0; i < numItems; i++) {
		_myGCodeHandler->setCursor(leftLineX + cursorOffsetX, startY - ((i + 2) * TODO_LINE_HEIGHT) + cursorOffsetY);
		_myGCodeHandler->write(itemList[i].name, WRAP_TRUNCATE, true);
	}

	if(hasLeftLabel) {
		_myGCodeHandler->setFontScale(TODO_LABEL_FONT_SCALE);
		_myGCodeHandler->setTextConstraints(startX, startY, startX + TODO_LEFT_LABEL_SPACE, endY);

		cursorOffsetY = (TODO_LINE_HEIGHT - LETTER_CAP_HEIGHT * _myGCodeHandler->getFontScale()) / 2.0;
		
		for(int i = 0; i < numItems; i++) {
			_myGCodeHandler->setCursor(startX, startY - ((i + 2) * TODO_LINE_HEIGHT) + cursorOffsetY);
			_myGCodeHandler->write(itemList[i].label, WRAP_TRUNCATE, true);
		}
	}
}