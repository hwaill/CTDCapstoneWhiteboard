#ifndef GCodeHandlerClass

#define GCodeHandlerClass
#include "Arduino.h"
#include "GCodeCommand.h"

class GCodeHandler {
	public:
		GCodeHandler(Stream &gcodeSerial, Stream &consoleSerial);

		void sendSingleGCODE(GCodeCommand command);
		void sendMultipleGCODE(GCodeCommand commands[], int numCommands);
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