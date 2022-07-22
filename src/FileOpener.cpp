
#include "FileOpener.h"
#include "Error.h"
#include <fstream>
#include <string>
#include <vector>

namespace NSEngine {

    std::ifstream FileOpener::file;

    bool FileOpener::Read(Bytestream &bs, const std::string &file)
    {
        return readFileToBuffer(file, bs);
    }

    bool FileOpener::Write(const Bytestream &bs, const std::string &file)
    {
        std::ofstream f(file, std::ios::binary);
        if (f.fail()) return false;
        f.write(reinterpret_cast<const char*>(&bs[0]), bs.size());
        f.close();
        info("Writing", bs.size(), "bytes to file :", file);
        return true;
    }

    bool FileOpener::readFileToBuffer(const std::string& filename, std::vector<char>& buffer)
    {
        std::ifstream f(filename, std::ios::binary);
        if (f.fail()) {
            return false;
        }

        //seek to the end
        f.seekg(0, std::ios::end);

        //Get the file size
        std::streamoff fileSize = f.tellg();
        f.seekg(0, std::ios::beg);

        //Reduce the file size by any header bytes
        fileSize -= f.tellg();

        buffer.resize((const size_t)fileSize);
        f.read(&(buffer[0]), fileSize);
        f.close();

        return true;
    }

    bool FileOpener::readFileToBuffer(const std::string& filename, std::vector<uint8_t>& buffer)
    {
        std::ifstream f(filename, std::ios::binary);
        if (f.fail()) {
            return false;
        }

        //seek to the end
        f.seekg(0, std::ios::end);

        //Get the file size
        std::streamoff fileSize = f.tellg();
        f.seekg(0, std::ios::beg);

        //Reduce the file size by any header bytes
        fileSize -= f.tellg();

        buffer.resize((const size_t)fileSize);
        f.read((char*)&(buffer[0]), fileSize);
        f.close();

        return true;
    }

    bool FileOpener::readFileToBuffer(const std::string& filename, std::string& buffer)
    {
        std::ifstream f(filename, std::ios::binary);
        if (f.fail()) {
            return false;
        }

        //seek to the end
        f.seekg(0, std::ios::end);

        //Get the file size
        std::streamoff fileSize = f.tellg();
        f.seekg(0, std::ios::beg);

        //Reduce the file size by any header bytes
        fileSize -= f.tellg();

        buffer.resize((const size_t) fileSize);
        f.read(&(buffer[0]), fileSize);
        f.close();

        return true;
    }

    bool FileOpener::readFileLines(const std::string& filename, std::vector<std::string>& lines)
    {
        std::ifstream f(filename);
        if (f.fail()) {
            return false;
        }
        std::string line;
        while (std::getline(f, line)) lines.push_back(line);
        return true;
    }

    bool FileOpener::openFile(const std::string& filename)
    {
        file.clear();
        file.close();
        std::ifstream f(filename, std::ios::binary);
        file.open(filename, std::ios::binary);

        if (file.fail()) return false;
        return true;
    }

    bool FileOpener::openFileLines(const std::string& filename)
    {
        file.clear();
        file.close();
        std::ifstream f(filename);
        file.open(filename);

        if (file.fail()) return false;
        return true;
    }

    bool FileOpener::readLine(std::string& l)
    {
        if(std::getline(file, l)) return true;
        return false;
    }

    void FileOpener::closeFile() { file.close(); }

    uint32_t FileOpener::getSize() { file.seekg(0, std::ios::end); return (uint32_t) file.tellg(); }


    uint8_t FileOpener::readUint8(int offset)
    {
        file.seekg(offset);
        uint8_t result = 0;
        file.read(reinterpret_cast<char*>(&result), 1);
        return result;
    }

    int8_t FileOpener::readInt8(int offset)
    {
        file.seekg(offset);
        int8_t result = 0;
        file.read(reinterpret_cast<char*>(&result), 1);
        return result;
    }

    uint16_t FileOpener::readUint16(int offset)
    {
        file.seekg(offset);
        uint16_t result = 0;
        file.read(reinterpret_cast<char*>(&result), 2);
        return result;
    }

    int16_t FileOpener::readInt16(int offset)
    {
        file.seekg(offset);
        int16_t result = 0;
        file.read(reinterpret_cast<char*>(&result), 2);
        return result;
    }

    uint32_t FileOpener::readUint32(int offset)
    {
        file.seekg(offset);
        uint32_t result = 0;
        file.read(reinterpret_cast<char*>(&result), 4);
        return result;
    }

    int32_t FileOpener::readInt32(int offset)
    {
        file.seekg(offset);
        int32_t result = 0;
        file.read(reinterpret_cast<char*>(&result), 4);
        return result;
    }

    union chartofloat {
        char in[4];
        float out;
    };

    float FileOpener::readFloat(int offset)
    {
        file.seekg(offset);
        chartofloat result;
        result.out = 0.f;
        file.read(result.in, 4);
        return result.out;
    }

    std::string FileOpener::readString(int offset, int& offsetEnd)
    {
        offsetEnd = offset;
        std::string thestring = {};
        char c = FileOpener::readInt8(offsetEnd);
        while (c != '\0')
        {
            thestring += c;
            offsetEnd++;
            c = FileOpener::readInt8(offsetEnd);
        }
        return thestring;
    }

}
