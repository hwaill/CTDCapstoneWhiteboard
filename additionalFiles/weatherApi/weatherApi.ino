// WiFiWeatherStation.ino
//#include "Arduino_LED_Matrix.h"
// #include "numbers.h"

#include "WiFiS3.h"
#include <Arduino_JSON.h>
#include <assert.h>

//#include "arduino_secrets.h"

unsigned long lastConnectionTime = 0;               // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10L * 1000L;  // delay between updates, in milliseconds
JSONVar myObject;
unsigned char frame[8][12];
int temperature = 0;
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "WifiDeluxe";  // your network SSID (name)
char pass[] = "fruitsandvegetables";  // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;           // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(202,61,229,161);  // numeric IP for api.open-meteo.com (no DNS)
char server[] = "api.open-meteo.com";  // name address for api.open-meteo.com (using DNS)
// https://api.open-meteo.com/v1/forecast?latitude=41.6561&longitude=-0.8773&current_weather=true&timezone=Europe%2FBerlin

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

/* -------------------------------------------------------------------------- */
void setup() {


  /* -------------------------------------------------------------------------- */
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }


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
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    //delay(10000);
  }
  // you're connected now, so print out the status:
  printWifiStatus();
}

/* just wrap the received data up to 80 columns in the serial print*/
/* -------------------------------------------------------------------------- */
void read_response() {
  /* -------------------------------------------------------------------------- */
  uint32_t received_data_num = 0;
  uint32_t data_num = 0;
  bool jsonDetected = false;
  char data[500];
  while (client.available() && data_num < 500) {
    /* actual data reception */
    char c = client.read();
    // json start detected
    if ('{' == c) {
      jsonDetected = true;
    }
    if (jsonDetected) {
      data[data_num++] = c;
    }
  }
  //data[received_data_num] = 0;
  if (jsonDetected) {
    // Serial.println(data);
    myObject = JSON.parse(data);
    Serial.print("Temperature F: ");
    if (myObject.hasOwnProperty("current_weather")) {

      temperature = (double)myObject["current_weather"]["temperature"];
      Serial.println(temperature);
    }
  }
}

/* -------------------------------------------------------------------------- */
void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  read_response();
  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
    http_request(40.014984, -105.270546);
  }
}

/* -------------------------------------------------------------------------- */
void printWifiStatus() {
  /* -------------------------------------------------------------------------- */
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



void http_request(float latitude, float longitude) {
  client.stop();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET /v1/forecast?latitude=" + String(latitude) + "&longitude=" + String(longitude) + "&temperature_unit=fahrenheit&current_weather=true HTTP/1.1");
    // https://api.open-meteo.com/v1/forecast?latitude=40.014984&longitude=-105.270546&daily=apparent_temperature_max&daily=apparent_temperature_min&daily=precipitation_probability_mean&forecast_days=1
    client.println("Host: api.open-meteo.com");
    client.println("Connection: close");
    client.println();
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}