#ifndef GCodeCommandClass
#define GCodeCommandClass
#include "GCodeCommandPair.h"

class GCodeCommand {
	public:
		GCodeCommand(GCodeCommandPair commandPairs[], int numPairs);
		GCodeCommand();

		static GCodeCommand createCommandFromString(String input);
		
		void addCommandPair(String type, double value);
		String toString();
		String toStringAtCoordinate(double x, double y);
	private:
		GCodeCommandPair _commandPairs[6];
		int _numPairs;
};

#endif