#include "GCodeHandler.h"

GCodeHandler::GCodeHandler(Stream &gcodeSerial, Stream &consoleSerial) {
	_gcodeSerial = &gcodeSerial;
	_consoleSerial = &consoleSerial;

	_cursorX = 0;
	_cursorY = 0;
}

String GCodeHandler::_SENT_HEADER = "SENT:     ";
String GCodeHandler::_RECV_HEADER = "RECEIVED: ";

void GCodeHandler::sendSingleGCODE(String command) {
	_consoleSerial->println(command);
	_gcodeSerial->print(command);
	_gcodeSerial->print('\n');
	String response = _waitGRBLSerial();
	_consoleSerial->print(response);
}

// void GCodeHandler::sendSingleGCODE(const char* command) {
// 	_consoleSerial->println(command);
// 	_gcodeSerial->print(command);
// 	_gcodeSerial->print('\n');
// 	String response = _waitGRBLSerial();
// 	_consoleSerial->println(response);
// }

void GCodeHandler::sendMultipleGCODE(char* commands[], int numCommands) {
	for(int i = 0; i < numCommands; i++) {
		sendSingleGCODE(commands[i]);
	}
}

void GCodeHandler::sendMultipleGCODE(const char* commands[], int numCommands) {
	for(int i = 0; i < numCommands; i++) {
		sendSingleGCODE(commands[i]);
	}
}

void GCodeHandler::initialize() {
	
	_consoleSerial->println("INIT");
	sendMultipleGCODE(GRBL_SETTINGS, 33);
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


String GCodeHandler::mapGCODEToPositionAndScale(gcodeCommandString command, double posX, double posY, double scale) {
	String output = "";
	String input = command.commandString;

	int numPairs = command.numPairs;

	String currentPairString;
	String type;
	double value;
	int cursor = 0;

	for(int i = 0; i < numPairs; i++) {
		if(input.substring(cursor).indexOf(" ") != -1) {
			currentPairString = input.substring(cursor, input.substring(cursor).indexOf(" ") + cursor);
		} else {
			currentPairString = input.substring(cursor);
		}

		cursor += input.substring(cursor).indexOf(" ") + 1;

		type = currentPairString.substring(0, 1);
		value = currentPairString.substring(1, 2).equals("-") ? currentPairString.substring(2).toDouble() * -1.0 : currentPairString.substring(1).toDouble();

		if(type.equals("X") || type.equals("Y") || type.equals("I") || type.equals("J") || type.equals("P") || type.equals("Q")) {
			value *= scale;
		}

		if(type.equals("X") || type.equals("I") || type.equals("P")) {
			value += posX;
		} else if(type.equals("Y") || type.equals("J") || type.equals("Q")) {
			value += posY;
		}

		output = output + type + (type.equals("G") ? String((int)(value)) : String(value, 5));

		if(i != numPairs - 1) {
			output = output + " ";
		}
	}

	return output;
}

void GCodeHandler::sendCharacterAtPositionAndScale(gcodeCommandString commands[], int numCommands, double posX, double posY, double scale) {
	for(int i = 0; i < numCommands; i++) {
		sendSingleGCODE(mapGCODEToPositionAndScale(commands[i], posX, posY, scale));
	}
}

void GCodeHandler::drawLine(double startX, double startY, double endX, double endY) {
	sendSingleGCODE("G00 Z0.2");

	String lineCommand = "G00 X";
	lineCommand += startX;
	lineCommand += " Y";
	lineCommand += startY;

	sendSingleGCODE(lineCommand);
	sendSingleGCODE("G00 Z-0.2");

	lineCommand = "G01 X";
	lineCommand += endX;
	lineCommand += " Y";
	lineCommand += endY;
	lineCommand += " F15000";

	sendSingleGCODE(lineCommand);
	sendSingleGCODE("G00 Z0.2");

	return;
}

void GCodeHandler::drawRect(double startX, double startY, double endX, double endY) {
	sendSingleGCODE("G00 Z0.2");

	String rectCommand = "G00 X";
	rectCommand += startX;
	rectCommand += " Y";
	rectCommand += startY;

	sendSingleGCODE(rectCommand);
	sendSingleGCODE("G00 Z-0.2");

	rectCommand = "G01 X";
	rectCommand += startX;
	rectCommand += " Y";
	rectCommand += endY;
	rectCommand += " F15000";

	sendSingleGCODE(rectCommand);

	rectCommand = "G01 X";
	rectCommand += endX;
	rectCommand += " Y";
	rectCommand += endY;
	rectCommand += " F15000";

	sendSingleGCODE(rectCommand);

	rectCommand = "G01 X";
	rectCommand += endX;
	rectCommand += " Y";
	rectCommand += startY;
	rectCommand += " F15000";

	sendSingleGCODE(rectCommand);

	rectCommand = "G01 X";
	rectCommand += startX;
	rectCommand += " Y";
	rectCommand += startY;
	rectCommand += " F15000";

	sendSingleGCODE(rectCommand);
	sendSingleGCODE("G00 Z0.2");

	return;
}

void GCodeHandler::drawCircle(double centerX, double centerY, double radius) {
	sendSingleGCODE("G00 Z0.2");

	String circleCommand = "G00 X";
	circleCommand += centerX - radius;
	circleCommand += " Y";
	circleCommand += centerY;

	sendSingleGCODE(circleCommand);
	sendSingleGCODE("G00 Z-0.2");

	circleCommand = "G02 I";
	circleCommand += radius;
	circleCommand += " F15000";

	sendSingleGCODE(circleCommand);
	sendSingleGCODE("G00 Z0.2");
}