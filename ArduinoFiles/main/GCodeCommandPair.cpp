#include "GCodeCommandPair.h";

GCodeCommandPair::GCodeCommandPair(String type, double value) {
	GCodeCommandPair(type, value, false);
}

GCodeCommandPair::GCodeCommandPair(String type, double value, boolean isPrimaryPair) {
	_type = type;
	_value = value;
	_isPrimaryPair = isPrimaryPair;
}

GCodeCommandPair GCodeCommandPair::createGCodeCommandPairFromString(String input, boolean isPrimaryPair) {
	String type = input.substring(0, 1);
	boolean isNegative = input.substring(1, 2).equals("-") ? true : false;
	double value = isNegative ? input.substring(2).toDouble() * -1 : input.substring(1).toDouble();

	return GCodeCommandPair(type, value, isPrimaryPair);
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