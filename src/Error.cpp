#include "Error.h"
#include "NSEngine.h"
#include "Engine.hpp"
#include <SDL2/SDL.h>
#include <cstdlib>
#include <string>

namespace NSEngine {

    MessagePrintLevel loglevel = MessagePrintLevel::LV_INFO;

    /*
    void fatalError(std::string errorString) 
    {
        if (loglevel >= MessagePrintLevel::LV_FATAL)
            std::cout << "[FATAL ERROR] : " << errorString << std::endl;
        Quit();
        exit(EXIT_FAILURE);
    }

    void error(std::string errorString)
    {
        if (loglevel >= MessagePrintLevel::LV_ERROR)
            std::cout << "[ERROR] : " << errorString << std::endl;
    }

    void warning(std::string warningString)
    {
        if (loglevel >= MessagePrintLevel::LV_WARN)
            std::cout << "[WARNING] : " << warningString << std::endl;
    }

    void info(std::string infoString)
    {
        if (loglevel >= MessagePrintLevel::LV_INFO)
            std::cout << "[INFO] : " << infoString << std::endl;
    }*/

    void setPrintLevel(MessagePrintLevel lvl) { loglevel = lvl; }
    MessagePrintLevel getPrintLevel() { return loglevel; }

    void FatalErrorQuit()
    {
        getInstance()->on_destroy();
        engineData::layers.clear();
        if (engineData::cam2d != nullptr) delete engineData::cam2d;
        engineData::cam2d = nullptr;
        if (engineData::cam3d != nullptr) delete engineData::cam3d;
        engineData::cam3d = nullptr;
        exit(EXIT_FAILURE);
    }
}
