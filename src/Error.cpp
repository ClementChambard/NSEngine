#include "Error.h"
#include <SDL2/SDL.h>
#include <cstdlib>

namespace ns {

MessagePrintLevel loglevel = MessagePrintLevel::LV_INFO;

void setPrintLevel(MessagePrintLevel lvl) { loglevel = lvl; }
MessagePrintLevel getPrintLevel() { return loglevel; }

void FatalErrorQuit() { exit(EXIT_FAILURE); } // TODO: Remove

}
