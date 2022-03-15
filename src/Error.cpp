#include "Error.h"
#include "NSEngine.h"
#include <SDL2/SDL.h>
#include <cstdlib>
#include <string>

namespace NSEngine {

    int loglevel = 0;

    void fatalError(std::string errorString) 
    {
        if (loglevel < 4)
            std::cout << "[FATAL ERROR] : " << errorString << std::endl;
        Quit();
        exit(EXIT_FAILURE);
    }

    void error(std::string errorString)
    {
        if (loglevel < 3)
            std::cout << "[ERROR] : " << errorString << std::endl;
    }

    void warning(std::string warningString)
    {
        if (loglevel < 2)
            std::cout << "[WARNING] : " << warningString << std::endl;
    }

    void info(std::string infoString)
    {
        if (loglevel < 1)
            std::cout << "[INFO] : " << infoString << std::endl;
    }

    void setPrintLevel(int lvl) { loglevel = lvl; }
}
