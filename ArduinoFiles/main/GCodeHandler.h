#ifndef GCodeHandlerClass

#define GCodeHandlerClass
#include "Arduino.h"
#include "GCodeCommand.h"
#include "GCodeLibrary.h"

class GCodeHandler {
	public:
		GCodeHandler(Stream &gcodeSerial, Stream &consoleSerial);

		void sendSingleGCODE(GCodeCommand command);
		void sendSingleGCODE(String command);
		void sendMultipleGCODE(GCodeCommand commands[], int numCommands);

		GCodeLibrary library;
	private:
		Stream* _gcodeSerial;
		Stream* _consoleSerial;

		static String _SENT_HEADER;
		static String _RECV_HEADER;
		String _waitGRBLSerial();
		void _emptyGRBLSerialBuffer();
		void _wakeGRBLSerial();
		void _sendSingleGCODE(GCodeCommand command);
		void _sendMultipleGCODE(GCodeCommand commands[], int numCommands);
};

#endif