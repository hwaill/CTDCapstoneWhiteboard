#include "GCodeCommand.h"
#include "GCodeCommandPair.h"

GCodeCommand::GCodeCommand() {
	
}

String GCodeCommand::toString() {
	String toReturn = "";

	for(int i = 0; i < _numPairs; i++) {
		toReturn += _commandPairs[i].toString();
		if(i != _numPairs - 1) {
			toReturn += " ";
		}
	}

	return toReturn;
}

String GCodeCommand::toStringAtCoordinate(double x, double y) {
	String toReturn = "";

	for(int i = 0; i < _numPairs; i++) {
		toReturn += _commandPairs[i].toStringAtCoordinate(x, y);
		if(i != _numPairs - 1) {
			toReturn += " ";
		}
	}

	return toReturn;
}

GCodeCommand_1::GCodeCommand_1(GCodeCommandPair commandPairs[1]) : GCodeCommand() {
	_commandPairs = commandPairs;
	_numPairs = 1;
}

GCodeCommand_2::GCodeCommand_2(GCodeCommandPair commandPairs[2]) : GCodeCommand() {
	_commandPairs = commandPairs;
	_numPairs = 2;
}

GCodeCommand_3::GCodeCommand_3(GCodeCommandPair commandPairs[3]) : GCodeCommand() {
	_commandPairs = commandPairs;
	_numPairs = 3;
}

GCodeCommand_4::GCodeCommand_4(GCodeCommandPair commandPairs[4]) : GCodeCommand() {
	_commandPairs = commandPairs;
	_numPairs = 4;
}

GCodeCommand_5::GCodeCommand_5(GCodeCommandPair commandPairs[5]) : GCodeCommand() {
	_commandPairs = commandPairs;
	_numPairs = 5;
}

GCodeCommand_6::GCodeCommand_6(GCodeCommandPair commandPairs[6]) : GCodeCommand() {
	_commandPairs = commandPairs;
	_numPairs = 6;
}

GCodeCommand_7::GCodeCommand_7(GCodeCommandPair commandPairs[7]) : GCodeCommand() {
	_commandPairs = commandPairs;
	_numPairs = 7;
}

GCodeCommand_8::GCodeCommand_8(GCodeCommandPair commandPairs[8]) : GCodeCommand() {
	_commandPairs = commandPairs;
	_numPairs = 8;
}