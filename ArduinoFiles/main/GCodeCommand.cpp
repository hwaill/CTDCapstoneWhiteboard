#include "GCodeCommand.h"
#include "GCodeCommandPair.h"

GCodeCommand::GCodeCommand() {
	
}

String GCodeCommand::toString() {
	return "A";
}

String GCodeCommand::toStringAtCoordinate(double x, double y) {
	return "B";
}

GCodeCommand_1::GCodeCommand_1(GCodeCommandPair commandPairs[1]) : GCodeCommand() {
	_numPairs = 1;
	for(int i = 0; i < _numPairs; i++) {
		_commandPairs[i] = commandPairs[i];
	}
}

GCodeCommand_2::GCodeCommand_2(GCodeCommandPair commandPairs[2]) : GCodeCommand() {
	_numPairs = 2;
	for(int i = 0; i < _numPairs; i++) {
		_commandPairs[i] = commandPairs[i];
	}
}

GCodeCommand_3::GCodeCommand_3(GCodeCommandPair commandPairs[3]) : GCodeCommand() {
	_numPairs = 3;
	for(int i = 0; i < _numPairs; i++) {
		_commandPairs[i] = commandPairs[i];
	}
}

GCodeCommand_4::GCodeCommand_4(GCodeCommandPair commandPairs[4]) : GCodeCommand() {
	_numPairs = 4;
	for(int i = 0; i < _numPairs; i++) {
		_commandPairs[i] = commandPairs[i];
	}
}

GCodeCommand_5::GCodeCommand_5(GCodeCommandPair commandPairs[5]) : GCodeCommand() {
	_numPairs = 5;
	for(int i = 0; i < _numPairs; i++) {
		_commandPairs[i] = commandPairs[i];
	}
}

GCodeCommand_6::GCodeCommand_6(GCodeCommandPair commandPairs[6]) : GCodeCommand() {
	_numPairs = 6;
	for(int i = 0; i < _numPairs; i++) {
		_commandPairs[i] = commandPairs[i];
	}
}

GCodeCommand_7::GCodeCommand_7(GCodeCommandPair commandPairs[7]) : GCodeCommand() {
	_numPairs = 7;
	for(int i = 0; i < _numPairs; i++) {
		_commandPairs[i] = commandPairs[i];
	}
}

GCodeCommand_8::GCodeCommand_8(GCodeCommandPair commandPairs[8]) : GCodeCommand() {
	_numPairs = 8;
	for(int i = 0; i < _numPairs; i++) {
		_commandPairs[i] = commandPairs[i];
	}
}

String GCodeCommand_1::toString() {
	String toReturn = "";
	for(int i = 0; i < _numPairs; i++) {
		toReturn += _commandPairs[i].toString();
		if(i != _numPairs - 1) {
			toReturn += " ";
		}
	}

	return toReturn;
}

String GCodeCommand_1::toStringAtCoordinate(double x, double y) {
	String toReturn = "";

	for(int i = 0; i < _numPairs; i++) {
		toReturn += _commandPairs[i].toStringAtCoordinate(x, y);
		if(i != _numPairs - 1) {
			toReturn += " ";
		}
	}

	return toReturn;
}

String GCodeCommand_2::toString() {
	String toReturn = "";
	for(int i = 0; i < _numPairs; i++) {
		toReturn += _commandPairs[i].toString();
		if(i != _numPairs - 1) {
			toReturn += " ";
		}
	}

	return toReturn;
}

String GCodeCommand_2::toStringAtCoordinate(double x, double y) {
	String toReturn = "";

	for(int i = 0; i < _numPairs; i++) {
		toReturn += _commandPairs[i].toStringAtCoordinate(x, y);
		if(i != _numPairs - 1) {
			toReturn += " ";
		}
	}

	return toReturn;
}

String GCodeCommand_3::toString() {
	String toReturn = "";
	for(int i = 0; i < _numPairs; i++) {
		toReturn += _commandPairs[i].toString();
		if(i != _numPairs - 1) {
			toReturn += " ";
		}
	}

	return toReturn;
}

String GCodeCommand_3::toStringAtCoordinate(double x, double y) {
	String toReturn = "";

	for(int i = 0; i < _numPairs; i++) {
		toReturn += _commandPairs[i].toStringAtCoordinate(x, y);
		if(i != _numPairs - 1) {
			toReturn += " ";
		}
	}

	return toReturn;
}

String GCodeCommand_4::toString() {
	String toReturn = "";
	for(int i = 0; i < _numPairs; i++) {
		toReturn += _commandPairs[i].toString();
		if(i != _numPairs - 1) {
			toReturn += " ";
		}
	}

	return toReturn;
}

String GCodeCommand_4::toStringAtCoordinate(double x, double y) {
	String toReturn = "";

	for(int i = 0; i < _numPairs; i++) {
		toReturn += _commandPairs[i].toStringAtCoordinate(x, y);
		if(i != _numPairs - 1) {
			toReturn += " ";
		}
	}

	return toReturn;
}

String GCodeCommand_5::toString() {
	String toReturn = "";
	for(int i = 0; i < _numPairs; i++) {
		toReturn += _commandPairs[i].toString();
		if(i != _numPairs - 1) {
			toReturn += " ";
		}
	}

	return toReturn;
}

String GCodeCommand_5::toStringAtCoordinate(double x, double y) {
	String toReturn = "";

	for(int i = 0; i < _numPairs; i++) {
		toReturn += _commandPairs[i].toStringAtCoordinate(x, y);
		if(i != _numPairs - 1) {
			toReturn += " ";
		}
	}

	return toReturn;
}

String GCodeCommand_6::toString() {
	String toReturn = "";
	for(int i = 0; i < _numPairs; i++) {
		toReturn += _commandPairs[i].toString();
		if(i != _numPairs - 1) {
			toReturn += " ";
		}
	}

	return toReturn;
}

String GCodeCommand_6::toStringAtCoordinate(double x, double y) {
	String toReturn = "";

	for(int i = 0; i < _numPairs; i++) {
		toReturn += _commandPairs[i].toStringAtCoordinate(x, y);
		if(i != _numPairs - 1) {
			toReturn += " ";
		}
	}

	return toReturn;
}

String GCodeCommand_7::toString() {
	String toReturn = "";
	for(int i = 0; i < _numPairs; i++) {
		toReturn += _commandPairs[i].toString();
		if(i != _numPairs - 1) {
			toReturn += " ";
		}
	}

	return toReturn;
}

String GCodeCommand_7::toStringAtCoordinate(double x, double y) {
	String toReturn = "";

	for(int i = 0; i < _numPairs; i++) {
		toReturn += _commandPairs[i].toStringAtCoordinate(x, y);
		if(i != _numPairs - 1) {
			toReturn += " ";
		}
	}

	return toReturn;
}

String GCodeCommand_8::toString() {
	String toReturn = "";
	for(int i = 0; i < _numPairs; i++) {
		toReturn += _commandPairs[i].toString();
		if(i != _numPairs - 1) {
			toReturn += " ";
		}
	}

	return toReturn;
}

String GCodeCommand_8::toStringAtCoordinate(double x, double y) {
	String toReturn = "";

	for(int i = 0; i < _numPairs; i++) {
		toReturn += _commandPairs[i].toStringAtCoordinate(x, y);
		if(i != _numPairs - 1) {
			toReturn += " ";
		}
	}

	return toReturn;
}