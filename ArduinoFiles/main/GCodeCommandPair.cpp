#include "GCodeCommandPair.h";

GCodeCommandPair::GCodeCommandPair(String type, double value) {
	_type = type;
	_value = value;
}

String GCodeCommandPair::toString() {
	String toReturn = "";

	toReturn += _type;
	toReturn += String(_value);

	return toReturn;
}

String GCodeCommandPair::toStringAtCoordinate(double x, double y) {
	String toReturn = "";

	toReturn += _type;
	if(_type.equals("X")) {
		toReturn += String(_value + x);
	} else if(_type.equals("Y")) {
		toReturn += String(_value + y);
	} else {
		toReturn += String(_value);
	}

	return toReturn;
}