#include "GCodeCommand.h"

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