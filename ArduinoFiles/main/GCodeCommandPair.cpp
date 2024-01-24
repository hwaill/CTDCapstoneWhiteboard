#include "GCodeCommandPair.h";

GCodeCommandPair::GCodeCommandPair(String type, double value) {
	GCodeCommandPair(type, value, false);
}

GCodeCommandPair::GCodeCommandPair(String type, double value, boolean isPrimaryPair) {
	_type = type;
	_value = value;
	_isPrimaryPair = isPrimaryPair;
}

String GCodeCommandPair::toString() {
	String toReturn = "";

	toReturn += _type;
	if(_isPrimaryPair) {
		toReturn += String((int)(_value));
	} else {
		toReturn += String(_value);
	}

	return toReturn;
}

String GCodeCommandPair::toStringAtCoordinate(double x, double y) {
	String toReturn = "";

	toReturn += _type;
	if(_isPrimaryPair) {
		toReturn += String((int)(_value));
	} else {
		if(_type.equals("X")) {
			toReturn += String(_value + x);
		} else if(_type.equals("Y")) {
			toReturn += String(_value + y);
		} else {
			toReturn += String(_value);
		}
	}

	return toReturn;
}