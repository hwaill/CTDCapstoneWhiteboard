#ifndef GCodeHandlerClass

#define GCodeHandlerClass
#include "Arduino.h"
#include "GCodeLibrary.cpp"

inline const int WRAP_WRAP = 0;
inline const int WRAP_TRUNCATE = 1;
inline const int WRAP_ELLIPSES = 2;

class GCodeHandler {
	public:
		GCodeHandler(Stream &gcodeSerial, Stream &consoleSerial);

		void initialize();

		void setCursor(double x, double y);
		void setFontScale(double scale);
		double getFontScale();
		void setTextConstraints(double startX, double startY, double endX, double endY);

		void returnToHome();

		void sendSingleCommand(String command);
		void sendSingleCommand(const char* command, double posX, double posY, double scale);
		void sendMultipleCommands(const char* commands[], int numCommands);
		void sendMultipleCommands(const char* commands[], int numCommands, double posX, double posY, double scale);
		void sendCharacter(const char c, double posX, double posY, double scale);
		void sendWord(const char* word);
		void write(const char* text, int wrapBehavior, bool obeyConstraints);
		void drawLine(double startX, double startY, double endX, double endY);
		void drawRect(double startX, double startY, double endX, double endY);
		void drawCircle(double centerX, double centerY, double radius);
		void boxZigZag(double startX, double startY, double endX, double endY, double rowHeight);

		static double CANVAS_START_X;
		static double CANVAS_START_Y;
		static double CANVAS_END_X;
		static double CANVAS_END_Y;
	private:
		Stream* _gcodeSerial;
		Stream* _consoleSerial;
		
		double _cursorX;
		double _cursorY;
		double _fontScale;

		double _textConstraintStartX;
		double _textConstraintStartY;
		double _textConstraintEndX;
		double _textConstraintEndY;

		static String _SENT_HEADER;
		static String _RECV_HEADER;

		void _sendSingleCommand(String command);
		void _sendSingleCommand(const char* command, double posX, double posY, double scale);
		void _sendMultipleCommands(const char* commands[], int numCommands);
		void _sendMultipleCommands(const char* commands[], int numCommands, double posX, double posY, double scale);
		void _sendMultipleCommandsWithTerminator(const char* commands[], double posX, double posY, double scale);
		void _sendCharacter(const char c, double posX, double posY, double scale);
		void _sendCharacterWithCursor(const char c);
		void _sendWord(const char* word);
		void _sendWord(String word);
		double _calculateWordWidth(const char* word);
		double _calculateWordWidth(String word);
		void _drawLine(double startX, double startY, double endX, double endY);
		void _drawLineNoLift(double endX, double endY);
		void _drawRect(double startX, double startY, double endX, double endY);
		void _drawCircle(double centerX, double centerY, double radius);
		void _boxZigZag(double startX, double startY, double endX, double endY, double rowHeight);

		String _waitGRBLSerial();
		void _emptyGRBLSerialBuffer();
		void _wakeGRBLSerial();
		
		String _mapGCODEToPositionAndScale(const char* command, double posX, double posY, double scale);
		int _mapCharToIndex(const char c);
		void _checkCursorInBounds(bool obeyConstraints);
};

#endif