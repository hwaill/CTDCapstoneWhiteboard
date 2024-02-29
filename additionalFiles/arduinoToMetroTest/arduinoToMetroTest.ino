const String SENT_HEADER = "SENT:     ";
const String RECV_HEADER = "RECEIVED: ";

void setup(){
  Serial.begin(115200);
	Serial1.begin(115200);

  pinMode(6, OUTPUT);

  digitalWrite(6, HIGH);
}

String userIn = "";

void loop(){
  //checks for user input
  while(Serial.available()) {
    userIn += (char)Serial.read();
    delay(2);
  }

  //send command to metro if there is
  if(!userIn.equals("")) {
    Serial1.print("\r\n\r\n");
    delay(2);
    emptyGRBLSerialBuffer();
    Serial.print(SENT_HEADER);
    Serial.print(userIn);
    Serial1.print(userIn);
    Serial.print(getGRBLSerial());
    userIn = "";
  }
}

void emptyGRBLSerialBuffer() {
  while(Serial1.available()) {
    Serial1.read();
  }
}

String getGRBLSerial(){
  String inLine = RECV_HEADER;
  char newChar;
	while(!Serial1.available()) {}

	while(Serial1.available()) {
    newChar = (char)Serial1.read();
    inLine += newChar;
    if(newChar == '\n' && Serial1.available()) {
      inLine += RECV_HEADER;
    }
    delay(2);
 	}

	return inLine;
}