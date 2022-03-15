#ifndef ERRORSDOTH
#define ERRORSDOTH

#include <iostream>
#include <string>

namespace NSEngine {

    extern void fatalError(std::string errorString);
    extern void error(std::string errorString);
    extern void warning(std::string warningString);
    extern void info(std::string infoString);

    extern void setPrintLevel(int lvl);
}

#endif
