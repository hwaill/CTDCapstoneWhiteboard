#include "GCodeHandler.h"

/*
	Constructor requires a serial stream over which to send gcode, and a serial stream to log to the console
*/
GCodeHandler::GCodeHandler(Stream &gcodeSerial, Stream &consoleSerial) {
	_gcodeSerial = &gcodeSerial;
	_consoleSerial = &consoleSerial;

	_cursorX = 0;
	_cursorY = 0;
	_fontScale = 1;

	_textConstraintStartX = CANVAS_START_X;
	_textConstraintStartY = CANVAS_START_Y;
	_textConstraintEndX = CANVAS_END_X;
	_textConstraintEndY = CANVAS_END_Y;
}

/*
	Static Variables
*/

//these are used for readability in the console
String GCodeHandler::_SENT_HEADER = "SENT:     ";
String GCodeHandler::_RECV_HEADER = "RECEIVED: ";

//these define the acceptable drawing area for the plotter
double GCodeHandler::CANVAS_START_X = 0;
double GCodeHandler::CANVAS_START_Y = 0;
double GCodeHandler::CANVAS_END_X = 863;
double GCodeHandler::CANVAS_END_Y = 563;

/*
	Public Functions
*/

//initializes the machine
void GCodeHandler::initialize() {
	_consoleSerial->println("Initializing Plotter and GRBL Configuration");
	_wakeGRBLSerial();
	_sendMultipleCommands(GRBL_SETTINGS, 33);
}

void GCodeHandler::setCursor(double x, double y) {
	_cursorX = x;
	_cursorY = y;
	
	_checkCursorInBounds(false);
}

void GCodeHandler::setFontScale(double scale) {
	_fontScale = scale;
}

void GCodeHandler::setTextConstraints(double startX, double startY, double endX, double endY) {
	_textConstraintStartX = startX;
	_textConstraintStartY = startY;
	_textConstraintEndX = endX;
	_textConstraintEndY = endY;
}

void GCodeHandler::sendSingleCommand(String command) {
	_wakeGRBLSerial();
	_sendSingleCommand(command);
}

void GCodeHandler::sendSingleCommand(const char* command, double posX, double posY, double scale) {
	_wakeGRBLSerial();
	_sendSingleCommand(command, posX, posY, scale);
}

void GCodeHandler::sendMultipleCommands(const char* commands[], int numCommands) {
	_wakeGRBLSerial();
	_sendMultipleCommands(commands, numCommands);
}

void GCodeHandler::sendMultipleCommands(const char* commands[], int numCommands, double posX, double posY, double scale) {
	_wakeGRBLSerial();
	_sendMultipleCommands(commands, numCommands, posX, posY, scale);
}

void GCodeHandler::sendCharacter(const char c, double posX, double posY, double scale) {
	_wakeGRBLSerial();
	_sendCharacter(c, posX, posY, scale);
}

void GCodeHandler::sendWord(const char* word) {
	_wakeGRBLSerial();
	_sendWord(word);
}

void GCodeHandler::write(const char* text, int wrapBehavior, boolean obeyConstraints) {
	double xStart = cursorX;
}

void GCodeHandler::drawLine(double startX, double startY, double endX, double endY) {
	_wakeGRBLSerial();
	_drawLine(startX, startY, endX, endY);
}

void GCodeHandler::drawRect(double startX, double startY, double endX, double endY) {
	_wakeGRBLSerial();
	_drawRect(startX, startY, endX, endY);
}

void GCodeHandler::drawCircle(double centerX, double centerY, double radius) {
	_wakeGRBLSerial();
	_drawCircle(centerX, centerY, radius);
}

/*
	Private Functions
*/

void GCodeHandler::_sendSingleCommand(String command) {
	_consoleSerial->println(command);
	_gcodeSerial->print(command);
	_gcodeSerial->print('\n');
	String response = _waitGRBLSerial();
	_consoleSerial->print(response);
}

void GCodeHandler::_sendSingleCommand(const char* command, double posX, double posY, double scale) {
	_sendSingleCommand(_mapGCODEToPositionAndScale(command, posX, posY, scale));
}

void GCodeHandler::_sendMultipleCommands(const char* commands[], int numCommands) {
	for(int i = 0; i < numCommands; i++) {
		_sendSingleCommand(commands[i]);
	}
}

void GCodeHandler::_sendMultipleCommands(const char* commands[], int numCommands, double posX, double posY, double scale) {
	for(int i = 0; i < numCommands; i++) {
		_sendSingleCommand(_mapGCODEToPositionAndScale(commands[i], posX, posY, scale));
	}
}

void GCodeHandler::_sendMultipleCommandsWithTerminator(const char* commands[], double posX, double posY, double scale) {
	int index = 0;
	while(commands[index] != "END") {
		_sendSingleCommand(_mapGCODEToPositionAndScale(commands[index++], posX, posY, scale));
	}
}

void GCodeHandler::_sendCharacter(const char c, double posX, double posY, double scale) {
	int index = _mapCharToIndex(c);
	
	if(index == -1) return;
	if(CHARACTER_WIDTHS[index] * scale + posX > CANVAS_END_X) return;
	if(LINE_HEIGHT * scale + posY > CANVAS_END_Y) return;
	if(MAX_DESCENDER * scale + posY < CANVAS_START_Y) return;

	_sendMultipleCommandsWithTerminator(CHARACTERS[index], posX, posY, scale);
}

void GCodeHandler::_sendCharacterWithCursor(const char c) {
	int index = _mapCharToIndex(c);
	
	if(index == -1) return;
	if(CHARACTER_WIDTHS[index] * _fontScale + _cursorX > CANVAS_END_X) return;
	if(LINE_HEIGHT * _fontScale + _cursorY > CANVAS_END_Y) return;
	if(MAX_DESCENDER * _fontScale + _cursorY < CANVAS_START_Y) return;

	_sendMultipleCommandsWithTerminator(CHARACTERS[index], _cursorX, _cursorY, _fontScale);
	
	_cursorX += CHARACTER_WIDTHS[index] * _fontScale;
}

void GCodeHandler::_sendWord(const char* word) {
	const char* c = &word[0];
	while(*c != '\0') {
    _sendCharacterWithCursor(*c);
		++c;
	}
}

double GCodeHandler::_calculateWordWidth(const char* word) {
	double width = 0;
	const char* c = &word[0];
	while(*c != '\0') {
		width += CHARACTER_WIDTHS[*c] * _fontScale;
	}

	return width;
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

String GCodeHandler::_mapGCODEToPositionAndScale(const char* command, double posX, double posY, double scale) {
	String output = "";
	String input = command;

	int numPairs = 1;
	int index = 0;
	while(command[index]) {
		if(command[index++] == ' ') {
			numPairs++;
		}
	}

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

		if(type.equals("X")) {
			value += posX;
		} else if(type.equals("Y")) {
			value += posY;
		}

		output = output + type + (type.equals("G") ? String((int)(value)) : String(value, 6));

		if(i != numPairs - 1) {
			output = output + " ";
		}
	}

	return output;
}

void GCodeHandler::_drawLine(double startX, double startY, double endX, double endY) {
	_sendSingleCommand("G00 Z0.2");

	String lineCommand = "G00 X";
	lineCommand += startX;
	lineCommand += " Y";
	lineCommand += startY;

	_sendSingleCommand(lineCommand);
	_sendSingleCommand("G00 Z-0.2");

	lineCommand = "G01 X";
	lineCommand += endX;
	lineCommand += " Y";
	lineCommand += endY;
	lineCommand += " F15000";

	_sendSingleCommand(lineCommand);
	_sendSingleCommand("G00 Z0.2");

	return;
}

void GCodeHandler::_drawRect(double startX, double startY, double endX, double endY) {
	_sendSingleCommand("G00 Z0.2");

	String rectCommand = "G00 X";
	rectCommand += startX;
	rectCommand += " Y";
	rectCommand += startY;

	_sendSingleCommand(rectCommand);
	_sendSingleCommand("G00 Z-0.2");

	rectCommand = "G01 X";
	rectCommand += startX;
	rectCommand += " Y";
	rectCommand += endY;
	rectCommand += " F15000";

	_sendSingleCommand(rectCommand);

	rectCommand = "G01 X";
	rectCommand += endX;
	rectCommand += " Y";
	rectCommand += endY;
	rectCommand += " F15000";

	_sendSingleCommand(rectCommand);

	rectCommand = "G01 X";
	rectCommand += endX;
	rectCommand += " Y";
	rectCommand += startY;
	rectCommand += " F15000";

	_sendSingleCommand(rectCommand);

	rectCommand = "G01 X";
	rectCommand += startX;
	rectCommand += " Y";
	rectCommand += startY;
	rectCommand += " F15000";

	_sendSingleCommand(rectCommand);
	_sendSingleCommand("G00 Z0.2");

	return;
}

void GCodeHandler::_drawCircle(double centerX, double centerY, double radius) {
	_sendSingleCommand("G00 Z0.2");

	String circleCommand = "G00 X";
	circleCommand += centerX - radius;
	circleCommand += " Y";
	circleCommand += centerY;

	_sendSingleCommand(circleCommand);
	_sendSingleCommand("G00 Z-0.2");

	circleCommand = "G02 X";
	circleCommand += centerX - radius;
	circleCommand += " Y";
	circleCommand += centerY;
	circleCommand += " I";
	circleCommand += radius;
	circleCommand += " F15000";

	_sendSingleCommand(circleCommand);
	_sendSingleCommand("G00 Z0.2");
}

int GCodeHandler::_mapCharToIndex(char c) {
	/* Indices for characters:
	0 - 25: A - Z
	26 - 51: a - z
	52 - 61: 0 - 9
	62: ,
	63: !
	64: .
	65: ?
	66: (space)
	*/
	int index;

	if(c >= 65 && c < 91) {
		index = c - 65;
	} else if(c >= 97 && c < 123) {
		index = c - 71;
	} else if(c >= 48 && c < 58) {
		index = c + 4;
	} else if (c == ',') {
		index = 62;
	} else if (c == '!') {
		index = 63;
	} else if (c == '.') {
		index = 64;
	} else if (c == '?') {
		index = 65;
	} else if (c == ' ') {
		index = 66;
	} else {
		index = -1;
	}

	return index;
}

void GCodeHandler::_checkCursorInBounds(boolean obeyConstraints) {
	double minX, maxX, minY, maxY;
	
	if(obeyConstraints) {
		minX = _textConstraintStartX;
		maxX = _textConstraintEndX;
		minY = _textConstraintStartY - MAX_DESCENDER * _fontScale;
		maxY = _textConstraintEndY - LINE_HEIGHT * _fontScale;
	} else {
		minX = CANVAS_START_X;
		maxX = CANVAS_END_X;
		minY = CANVAS_START_Y - MAX_DESCENDER * _fontScale;
		maxY = CANVAS_END_Y - LINE_HEIGHT * _fontScale;
	}
	if(_cursorX < minX) {
		_cursorX = minX;
	} else if(_cursorX > maxX) {
		_cursorX = maxX;
	}

	if(_cursorY > maxY - LINE_HEIGHT * _fontScale) {
		_cursorY = maxY - LINE_HEIGHT * _fontScale;
	} else if(_cursorY < minX - MAX_DESCENDER * _fontScale) {
		_cursorY = minX - MAX_DESCENDER * _fontScale;
	}
}