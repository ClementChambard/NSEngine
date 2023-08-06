#ifndef FILEOPENER
#define FILEOPENER


#include <vector>
#include <iostream>
#include <fstream>
#include "Bytestreams.h"

namespace NSEngine {

    class FileOpener
    {
        public:
            static bool Read(Bytestream& bs, std::string const& file);
            static bool Write(Bytestream const& bs, std::string const& file);


            static bool readFileToBuffer(const std::string& filename, void* &data, int& size);
            static bool readFileToBuffer(const std::string& filename, std::vector<char>& buffer);
            static bool readFileToBuffer(const std::string& filename, std::vector<uint8_t>& buffer);
            static bool readFileToBuffer(const std::string& filename, std::string& buffer);
            static bool readFileLines(const std::string& filename, std::vector<std::string>& lines);

            static bool openFile(const std::string& filename);
            static bool openFileLines(const std::string& filename);
            static void closeFile();
            static uint32_t getSize();

            static bool readLine(std::string& l);
            static uint8_t readUint8(int offset);
            static int8_t readInt8(int offset);
            static uint16_t readUint16(int offset);
            static int16_t readInt16(int offset);
            static uint32_t readUint32(int offset);
            static int32_t readInt32(int offset);
            static float readFloat(int offset);
        
            static std::string readString(int offset, int& offsetEnd);

        private:
            static std::ifstream file;

    };

}

#endif
