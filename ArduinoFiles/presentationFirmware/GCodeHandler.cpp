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
double GCodeHandler::CANVAS_END_Y = 563; //really 563

/*
	Public Functions
*/

//initializes the machine
void GCodeHandler::initialize() {
	_consoleSerial->println("Initializing Plotter and GRBL Configuration");
	_wakeGRBLSerial();
	_sendMultipleCommands(GRBL_SETTINGS, 33);
	_sendSingleCommand("$X");
	_sendSingleCommand("G10 P0 L20 X-24 Y-70 Z0.2");
	_sendSingleCommand("$H");
	_sendSingleCommand("G10 P0 L20 X0 Y-40 Z0.2");
	_sendSingleCommand("G00 X0 Y0");
	digitalWrite(8, LOW);
}

void GCodeHandler::setCursor(double x, double y) {
	_cursorX = x;
	_cursorY = y;
	
	_checkCursorInBounds(false);
}

void GCodeHandler::setFontScale(double scale) {
	_fontScale = scale;
}

double GCodeHandler::getFontScale() {
	return _fontScale;
}

void GCodeHandler::setTextConstraints(double startX, double startY, double endX, double endY) {
	_textConstraintStartX = startX;
	_textConstraintStartY = startY;
	_textConstraintEndX = endX;
	_textConstraintEndY = endY;
}

void GCodeHandler::returnToHome() {
	sendSingleCommand("G00 X0 Y0");
}

void GCodeHandler::sendSingleCommand(String command) {
	_wakeGRBLSerial();
	_sendSingleCommand(command);
	digitalWrite(8, LOW);
}

void GCodeHandler::sendSingleCommand(const char* command, double posX, double posY, double scale) {
	_wakeGRBLSerial();
	_sendSingleCommand(command, posX, posY, scale);
	digitalWrite(8, LOW);
}

void GCodeHandler::sendMultipleCommands(const char* commands[], int numCommands) {
	_wakeGRBLSerial();
	_sendMultipleCommands(commands, numCommands);
	digitalWrite(8, LOW);
}

void GCodeHandler::sendMultipleCommands(const char* commands[], int numCommands, double posX, double posY, double scale) {
	_wakeGRBLSerial();
	_sendMultipleCommands(commands, numCommands, posX, posY, scale);
	digitalWrite(8, LOW);
}

void GCodeHandler::sendCharacter(const char c, double posX, double posY, double scale) {
	_wakeGRBLSerial();
	_sendCharacter(c, posX, posY, scale);
	digitalWrite(8, LOW);
}

void GCodeHandler::sendWord(const char* word) {
	_wakeGRBLSerial();
	_sendWord(word);
	digitalWrite(8, LOW);
}

void GCodeHandler::write(String text, int wrapBehavior, bool obeyConstraints) {
	int stringLength = text.length() + 1;
	char textChars[stringLength];
	text.toCharArray(textChars, stringLength);

	write(textChars, wrapBehavior, obeyConstraints);
}

void GCodeHandler::write(const char* text, int wrapBehavior, bool obeyConstraints) {
	double xMax = obeyConstraints ? _textConstraintEndX : CANVAS_END_X;
	double yMin = obeyConstraints ? _textConstraintStartY : CANVAS_START_Y;
	String textToWrite = text;
	int numWords = 1;
	int index = 0;
	while(text[index]) {
		if(text[index++] == ' ') {
			numWords++;
		}
	}

	_wakeGRBLSerial();

	double xStart = _cursorX;
	double yStart = _cursorY;

	for(int i = 0; i < numWords; i++) {
		if((i < numWords - 1 && (_cursorX + _calculateWordWidth(textToWrite.substring(0, textToWrite.indexOf(' '))) > xMax)) || (i == numWords - 1 && (_cursorX + _calculateWordWidth(textToWrite.substring(0)) > xMax))) {
			//word needs to wrap
			if(wrapBehavior == WRAP_WRAP) {
				if(_cursorY - (LINE_HEIGHT * _fontScale) < yMin) {
					return;
				}

				_cursorY -= (LINE_HEIGHT * _fontScale);
				_cursorX = xStart;
			} else if(wrapBehavior == WRAP_TRUNCATE) {
				int lettersToInclude = 0;
				while(_cursorX + _calculateWordWidth(textToWrite.substring(0, lettersToInclude + 1)) < xMax) {
					lettersToInclude++;
				}
				_sendWord(textToWrite.substring(0, lettersToInclude));
				return;
			} else if(wrapBehavior == WRAP_ELLIPSES) {
				double ellipsesLength = _calculateWordWidth("...");
				int lettersToInclude = 0;
				while(_cursorX + ellipsesLength + _calculateWordWidth(textToWrite.substring(0, lettersToInclude + 1)) < xMax) {
					lettersToInclude++;
				}
				if(lettersToInclude == 0) {
					_cursorX -= _calculateWordWidth(" ");
				}
				String toSend = textToWrite.substring(0, lettersToInclude) + "...";
				_sendWord(toSend);
				return;
			}
		}

		if(i < numWords - 1) {
			_sendWord(textToWrite.substring(0, textToWrite.indexOf(' ') + 1));
			textToWrite = textToWrite.substring(textToWrite.indexOf(' ') + 1);
		} else {
			_sendWord(textToWrite.substring(0));
		}
	}
	digitalWrite(8, LOW);
}

void GCodeHandler::drawLine(double startX, double startY, double endX, double endY) {
	_wakeGRBLSerial();
	_drawLine(startX, startY, endX, endY);
	digitalWrite(8, LOW);
}

void GCodeHandler::drawRect(double startX, double startY, double endX, double endY) {
	_wakeGRBLSerial();
	_drawRect(startX, startY, endX, endY);
	digitalWrite(8, LOW);
}

void GCodeHandler::drawCircle(double centerX, double centerY, double radius) {
	_wakeGRBLSerial();
	_drawCircle(centerX, centerY, radius);
	digitalWrite(8, LOW);
}

void GCodeHandler::boxZigZag(double startX, double startY, double endX, double endY, double rowHeight) {
	_wakeGRBLSerial();
	_boxZigZag(startX, startY, endX, endY, rowHeight);
	digitalWrite(8, LOW);
}

/*
	Private Functions
*/

void GCodeHandler::_sendSingleCommand(String command) {
	digitalWrite(8, HIGH);
	//_consoleSerial->println(command);
	_gcodeSerial->print(command);
	_gcodeSerial->print('\n');
	String response = _waitGRBLSerial();
	//_consoleSerial->print(response);
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

void GCodeHandler::_sendWord(String word) {
	int stringLength = word.length() + 1;
	char wordChars[stringLength];
	word.toCharArray(wordChars, stringLength);

	_sendWord(wordChars);
}

double GCodeHandler::_calculateWordWidth(const char* word) {
	double width = 0;
	const char* c = &word[0];
	while(*c != '\0') {
		width += CHARACTER_WIDTHS[_mapCharToIndex(*c++)] * _fontScale;
	}
	return width;
}

double GCodeHandler::_calculateWordWidth(String word) {
	int stringLength = word.length() + 1;
	char wordChars[stringLength];
	word.toCharArray(wordChars, stringLength);

	return _calculateWordWidth(wordChars);
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
}

void GCodeHandler::_drawLineNoLift(double endX, double endY) {
	String lineCommand = "G01 X";
	lineCommand += endX;
	lineCommand += " Y";
	lineCommand += endY;
	lineCommand += " F15000";

	_sendSingleCommand(lineCommand);
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

void GCodeHandler::_boxZigZag(double startX, double startY, double endX, double endY, double rowHeight) {
	String lineCommand = "G00 X";
	lineCommand += startX;
	lineCommand += " Y";
	lineCommand += startY;

	_sendSingleCommand(lineCommand);
	_sendSingleCommand("G00 Z-0.2");

	bool lowToHigh = startY < endY;
	
	double currentY = startY;
	bool forward = true;

	while((lowToHigh && currentY <= endY) || (!lowToHigh && currentY >= endY)) {
		if(forward) {
			_drawLineNoLift(startX, currentY);
			_drawLineNoLift(endX, currentY);
		} else {
			_drawLineNoLift(endX, currentY);
			_drawLineNoLift(startX, currentY);
		}

		currentY += rowHeight;
		forward = !forward;
	}

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
	} else if (c == '\'') {
		index = 67;
	} else {
		index = -1;
	}

	return index;
}

void GCodeHandler::_checkCursorInBounds(bool obeyConstraints) {
	double minX, maxX, minY, maxY;
	
	if(obeyConstraints) {
		minX = _textConstraintStartX;
		maxX = _textConstraintEndX;
		minY = _textConstraintStartY - MAX_DESCENDER * _fontScale;
		maxY = _textConstraintEndY - LETTER_CAP_HEIGHT * _fontScale;
	} else {
		minX = CANVAS_START_X;
		maxX = CANVAS_END_X;
		minY = CANVAS_START_Y - MAX_DESCENDER * _fontScale;
		maxY = CANVAS_END_Y - LETTER_CAP_HEIGHT * _fontScale;
	}
	if(_cursorX < minX) {
		_cursorX = minX;
	} else if(_cursorX > maxX) {
		_cursorX = maxX;
	}

	if(_cursorY > maxY - LETTER_CAP_HEIGHT * _fontScale) {
		_cursorY = maxY - LETTER_CAP_HEIGHT * _fontScale;
	} else if(_cursorY < minX - MAX_DESCENDER * _fontScale) {
		_cursorY = minX - MAX_DESCENDER * _fontScale;
	}
}