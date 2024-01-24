#ifndef GCodeCommandPairClass
#define GCodeCommandPairClass
#include "Arduino.h"

class GCodeCommandPair {
	public:
		GCodeCommandPair(String type, double value);
		GCodeCommandPair(String type, double value, boolean isPrimaryPair);
		String toString();
		String toStringAtCoordinate(double x, double y);
	private:
		boolean _isPrimaryPair;
		String _type;
		double _value;
};

#endif