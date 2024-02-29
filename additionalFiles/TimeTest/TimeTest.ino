// Arduino R4 WiFi Get time from NTP
// Include the RTC library
#include "RTC.h"
//Include the NTP library
#include <NTPClient.h>
#include <WiFiS3.h>

#include <WiFiUdp.h>

int NTPCheck;
int NTPdelay;
int NTPdiff = 0;

///////Enter login data in the Secret tab/arduino_secrets.h
char ssid[] = "network";  // network SSID
char pass[] = "password";  // network password

int wifiStatus = WL_IDLE_STATUS;
WiFiUDP Udp;  // A UDP instance to let us send and receive packets over UDP
NTPClient timeClient(Udp);

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void connectToWiFi() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (wifiStatus != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network.
    wifiStatus = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("Connected to WiFi");
  printWifiStatus();
}

void NTP() {
  // Get the current date and time from an NTP server and convert
  // Time Zone offset for Cairns is 10 hours
  auto timeZoneOffsetHours = 10;
  auto unixTime = timeClient.getEpochTime() + (timeZoneOffsetHours * 3600);
  RTCTime timeToSet = RTCTime(unixTime);
  RTC.setTime(timeToSet);
}

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  connectToWiFi();
  RTC.begin();
  Serial.println("\nStarting connection to server...");
  timeClient.begin();
  timeClient.update();

  // Get the current time from NTP
  NTP();
}

void loop() {
  RTCTime currentTime;
  // Get current time from RTC
  RTC.getTime(currentTime);

  // Print out date (DD/MM//YYYY)
  Serial.print(currentTime.getDayOfMonth());
  Serial.print("/");
  Serial.print(Month2int(currentTime.getMonth()));
  Serial.print("/");
  Serial.print(currentTime.getYear());
  Serial.print(" - ");

  // Print time (HH/MM/SS)
  Serial.print(currentTime.getHour());
  Serial.print(":");
  Serial.print(currentTime.getMinutes());
  Serial.print(":");
  Serial.println(currentTime.getSeconds());

  delay(1000);

  // After 10 minutes, call NTP to set the RTC time again
  if (NTPdiff == 0) {
    NTPCheck = currentTime.getMinutes();
    NTPdelay = currentTime.getMinutes();
    NTPdiff = 1;
  } else {
    NTPdelay = currentTime.getMinutes();
    if (NTPdelay - NTPCheck <= 9) {
    } else {
      NTP();
      NTPdiff = 0;
      Serial.println("NTP Updated");
    }
  }
}