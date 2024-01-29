#ifndef GCodeHandlerClass

#define GCodeHandlerClass
#include "Arduino.h"
#include "GCodeLibrary.cpp"

class GCodeHandler {
	public:
		GCodeHandler(Stream &gcodeSerial, Stream &consoleSerial);

		void sendSingleGCODE(String command);
		void sendMultipleGCODE(String commands[], int numCommands);
		String _mapGCODEToPositionAndScale(gcodeCommandString command, double posX, double posY, double scaleX);
	private:
		Stream* _gcodeSerial;
		Stream* _consoleSerial;

		static String _SENT_HEADER;
		static String _RECV_HEADER;
		String _waitGRBLSerial();
		void _emptyGRBLSerialBuffer();
		void _wakeGRBLSerial();
		int _mapCharToIndex(char c);
		void _sendSingleGCODE(String command);
		void _sendMultipleGCODE(String commands[], int numCommands);
};

#endif