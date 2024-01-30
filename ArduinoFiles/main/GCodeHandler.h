#ifndef GCodeHandlerClass

#define GCodeHandlerClass
#include "Arduino.h"
#include "GCodeLibrary.cpp"

class GCodeHandler {
	public:
		GCodeHandler(Stream &gcodeSerial, Stream &consoleSerial);

		void sendSingleGCODE(char* command);
		void sendSingleGCODE(const char* command);
		void sendMultipleGCODE(char* commands[], int numCommands);
		void sendMultipleGCODE(const char* commands[], int numCommands);
		String _mapGCODEToPositionAndScale(gcodeCommandString command, double posX, double posY, double scaleX);
		void initialize();
	private:
		Stream* _gcodeSerial;
		Stream* _consoleSerial;

		static String _SENT_HEADER;
		static String _RECV_HEADER;
		String _waitGRBLSerial();
		void _emptyGRBLSerialBuffer();
		void _wakeGRBLSerial();
		int _mapCharToIndex(char c);
};

#endif