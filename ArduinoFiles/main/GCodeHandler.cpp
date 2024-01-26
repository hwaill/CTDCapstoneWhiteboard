#include "GCodeHandler.h"
#include "GCodeLibrary.h"

GCodeHandler::GCodeHandler(Stream &gcodeSerial, Stream &consoleSerial) {
	_gcodeSerial = &gcodeSerial;
	_consoleSerial = &consoleSerial;
}

String GCodeHandler::_SENT_HEADER = "SENT:     ";
String GCodeHandler::_RECV_HEADER = "RECEIVED: ";

void GCodeHandler::sendSingleGCODE(GCodeCommand command) {
	_wakeGRBLSerial();
	_sendSingleGCODE(command);
}

void GCodeHandler::sendMultipleGCODE(GCodeCommand commands[], int numCommands) {
	_wakeGRBLSerial();
	_sendMultipleGCODE(commands, numCommands);
}

void GCodeHandler::_sendSingleGCODE(GCodeCommand command) {
	_consoleSerial->println(command.toString());
	_gcodeSerial->print(command.toString());
	_gcodeSerial->print('\n');
	String response = _waitGRBLSerial();
	_consoleSerial->println(response);

	if(response.equals("ok")) return;
}

void GCodeHandler::sendSingleGCODE(String command) {
	_consoleSerial->println(command);
	_gcodeSerial->print(command);
	_gcodeSerial->print('\n');
	String response = _waitGRBLSerial();
	_consoleSerial->println(response);

	if(response.equals("ok")) return;
}

void GCodeHandler::_sendMultipleGCODE(GCodeCommand commands[], int numCommands) {
	for(int i = 0; i < numCommands; i++) {
		_sendSingleGCODE(commands[i]);
	}
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