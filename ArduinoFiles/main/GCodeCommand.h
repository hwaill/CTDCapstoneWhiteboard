#ifndef GCodeCommandClass
#define GCodeCommandClass
#include "GCodeCommandPair.h"

class GCodeCommand {
	public:
		GCodeCommand();
		
		virtual String toString();
		virtual String toStringAtCoordinate(double x, double y);
		int _numPairs;
};

class GCodeCommand_1 : public GCodeCommand {
	public:
		GCodeCommand_1(GCodeCommandPair commandPairs[1]);
		GCodeCommandPair _commandPairs[1];
		String toString();
		String toStringAtCoordinate(double x, double y);
};

class GCodeCommand_2 : public GCodeCommand {
	public:
		GCodeCommand_2(GCodeCommandPair commandPairs[2]);
		GCodeCommandPair _commandPairs[2];
		String toString();
		String toStringAtCoordinate(double x, double y);
};

class GCodeCommand_3 : public GCodeCommand {
	public:
		GCodeCommand_3(GCodeCommandPair commandPairs[3]);
		GCodeCommandPair _commandPairs[3];
		String toString();
		String toStringAtCoordinate(double x, double y);
};

class GCodeCommand_4 : public GCodeCommand {
	public:
		GCodeCommand_4(GCodeCommandPair commandPairs[4]);
		GCodeCommandPair _commandPairs[4];
		String toString();
		String toStringAtCoordinate(double x, double y);
};

class GCodeCommand_5 : public GCodeCommand {
	public:
		GCodeCommand_5(GCodeCommandPair commandPairs[5]);
		GCodeCommandPair _commandPairs[5];
		String toString();
		String toStringAtCoordinate(double x, double y);
};

class GCodeCommand_6 : public GCodeCommand {
	public:
		GCodeCommand_6(GCodeCommandPair commandPairs[6]);
		GCodeCommandPair _commandPairs[6];
		String toString();
		String toStringAtCoordinate(double x, double y);
};

class GCodeCommand_7 : public GCodeCommand {
	public:
		GCodeCommand_7(GCodeCommandPair commandPairs[7]);
		GCodeCommandPair _commandPairs[7];
		String toString();
		String toStringAtCoordinate(double x, double y);
};

class GCodeCommand_8 : public GCodeCommand {
	public:
		GCodeCommand_8(GCodeCommandPair commandPairs[8]);
		GCodeCommandPair _commandPairs[8];
		String toString();
		String toStringAtCoordinate(double x, double y);
};

#endif