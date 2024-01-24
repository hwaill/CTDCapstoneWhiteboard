#ifndef GCodeCommandPairClass
#define GCodeCommandPairClass
#include "Arduino.h"

class GCodeCommandPair {
	public:
		GCodeCommandPair();
		GCodeCommandPair(String type, double value);
		String toString();
		String toStringAtCoordinate(double x, double y);
		
	private:
		String _type;
		double _value;
};

#endif