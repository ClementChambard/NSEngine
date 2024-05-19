#ifndef FILEOPENER
#define FILEOPENER

#include <vector>
#include <string>
#include "Bytestreams.h"
#include "./defines.h"

namespace ns {

class FileOpener
{
    public:
        static bool Read(Bytestream& bs, cstr file);
        static bool Write(Bytestream const& bs, cstr file);

        static bool readFileToBuffer(cstr filename, ptr &data, i32& size);
        static bool readFileToBuffer(cstr filename, std::vector<char>& buffer);
        static bool readFileToBuffer(cstr filename, std::vector<u8>& buffer);
        static bool readFileToBuffer(cstr filename, std::string& buffer);
        static bool readFileLines(cstr filename, std::vector<std::string>& lines);

        static bool openFile(cstr filename);
        static bool openFileLines(cstr filename);
        static void closeFile();
        static u32 getSize();

        static bool readLine(std::string& l);
        static u8 readUint8(i32 offset);
        static i8 readInt8(i32 offset);
        static u16 readUint16(i32 offset);
        static i16 readInt16(i32 offset);
        static u32 readUint32(i32 offset);
        static i32 readInt32(i32 offset);
        static f32 readFloat(i32 offset);
    
        static std::string readString(int offset, int& offsetEnd);
};

}

#endif
