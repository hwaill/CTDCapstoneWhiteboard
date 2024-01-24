#include "GCodeHandler.h"
#include "Arduino.h"

GCodeHandler::GCodeHandler(Stream &gcodeSerial, Stream &consoleSerial) {
	_gcodeSerial = &gcodeSerial;
	_consoleSerial = &consoleSerial;
}

String GCodeHandler::_SENT_HEADER = "SENT:     ";
String GCodeHandler::_RECV_HEADER = "RECEIVED: ";

void GCodeHandler::sendSingleGCODE(GCodeCommand command) {
	_gcodeSerial->println(command.toString());
}

void GCodeHandler::_sendMultipleGCODE(GCodeCommand commands[]) {

}

void GCodeHandler::_wakeGRBLSerial() {
	_gcodeSerial->print("\r\n\r\n");
	delay(2);
  _emptyGRBLSerialBuffer();
}

String GCodeHandler::_waitGRBLSerial() {
	String inLine = "";
	while(!_gcodeSerial->available()) {}

	while(_gcodeSerial->available()) {
		inLine += (char)_gcodeSerial->read();
		delay(1);
	}

	return inLine;
}

void GCodeHandler::_emptyGRBLSerialBuffer() {
	while(_gcodeSerial->available()) {
    _gcodeSerial->read();
  }
}