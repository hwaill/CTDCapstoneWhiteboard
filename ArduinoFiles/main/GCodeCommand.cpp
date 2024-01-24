#include "GCodeCommand.h"

GCodeCommand::GCodeCommand() {
	GCodeCommand({}, 0);
}

GCodeCommand::GCodeCommand(GCodeCommandPair commandPairs[], int numPairs) {
	_numPairs = numPairs;
	for(int i = 0; i < numPairs; i++) {
		_commandPairs[i] = commandPairs[i];
	}
}

void GCodeCommand::addCommandPair(String type, double value) {
	if(_numPairs < 6) {
		if(_numPairs == 0) {
			_commandPairs[_numPairs++] = GCodeCommandPair(type, value, true);
		} else {
			_commandPairs[_numPairs++] = GCodeCommandPair(type, value);
		}
	}
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