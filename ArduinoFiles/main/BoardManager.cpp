#include "BoardManager.h"

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
	_checkForWifiInfo();
	if(_hasWiFiInfo) {
		_connectToWifi();
	}
	RTC.begin();
	_consoleSerial->println("\nStarting connection to server...");
	_timeClient->begin();
	_timeClient->update();

	// Get the current time from NTP
  NTP();
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