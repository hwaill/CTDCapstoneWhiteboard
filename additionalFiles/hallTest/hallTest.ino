const int MULTI_SELECT0 = 2;
const int MULTI_SELECT1 = 3;
const int MULTI_SELECT2 = 4;
const int MULTI_SELECT3 = 5;

//Multiplexer signal pins
const int SIGNAL_HALL_MULTI1 = A4;
const int SIGNAL_HALL_MULTI2 = A3;
const int SIGNAL_HALL_MULTI3 = A2;
const int SIGNAL_HALL_MULTI4 = A1;
const int SIGNAL_BUTTON_MULTI = A0;

int hallSensorValues[64];
bool hallSensorStates[64];

bool buttonStates[16];

void setup() {
	Serial.begin(115200);

	pinMode(MULTI_SELECT0, OUTPUT);
	pinMode(MULTI_SELECT1, OUTPUT);
	pinMode(MULTI_SELECT2, OUTPUT);
	pinMode(MULTI_SELECT3, OUTPUT);

	pinMode(SIGNAL_HALL_MULTI1, INPUT);
	pinMode(SIGNAL_HALL_MULTI2, INPUT);
	pinMode(SIGNAL_HALL_MULTI3, INPUT);
	pinMode(SIGNAL_HALL_MULTI4, INPUT);

	pinMode(8, OUTPUT);
}

void loop() {
	updateHallSensorValues();
	for(int i = 0; i < 64; i++) {
		// if(i == 66 || i == 57 || i == 34 || i == 35 || i == 36 || i == 37 || i == 38) {
		// 	continue;
		// }
		Serial.print(hallSensorStates[i] ? "1" : "0");
		if((i + 1) % 8 == 0) {
			Serial.println();
		}
	}
	delay(500);
}

void updateHallSensorValues() {
	for(int i = 0; i < 16; i++) {
    digitalWrite(MULTI_SELECT0, bitRead(i, 0));
    digitalWrite(MULTI_SELECT1, bitRead(i, 1));
    digitalWrite(MULTI_SELECT2, bitRead(i, 2));
    digitalWrite(MULTI_SELECT3, bitRead(i, 3));
    delayMicroseconds(63);

		hallSensorValues[i] = analogRead(SIGNAL_HALL_MULTI1);
		hallSensorValues[i + 16] = analogRead(SIGNAL_HALL_MULTI2);
		hallSensorValues[i + 32] = analogRead(SIGNAL_HALL_MULTI3);
		hallSensorValues[i + 48] = analogRead(SIGNAL_HALL_MULTI4);
  }

	for(int i = 0; i < 64; i++) {
		hallSensorStates[i] = hallSensorValues[i] > 560 || hallSensorValues[i] < 440;
	}
}

void updateButtonStates() {
	for(int i = 0; i < 16; i++) {
    digitalWrite(MULTI_SELECT0, bitRead(i, 0));
    digitalWrite(MULTI_SELECT1, bitRead(i, 1));
    digitalWrite(MULTI_SELECT2, bitRead(i, 2));
    digitalWrite(MULTI_SELECT3, bitRead(i, 3));
    delayMicroseconds(63);

		buttonStates[i] = digitalRead(SIGNAL_BUTTON_MULTI);
  }
}