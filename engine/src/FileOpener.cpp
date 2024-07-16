
#include "FileOpener.h"
#include <fstream>
#include <string>
#include <vector>

namespace ns {

static std::ifstream file;

bool FileOpener::read_file_to_buffer(cstr filename, ptr &data, i32& size)
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

    size = fileSize;
    data = malloc(fileSize);
    f.read(reinterpret_cast<pstr>(data), fileSize);
    f.close();

    return true;
}

bool FileOpener::read_file_to_buffer(cstr filename, std::vector<char>& buffer)
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

    buffer.resize((usize)fileSize);
    f.read(&(buffer[0]), fileSize);
    f.close();

    return true;
}

bool FileOpener::read_file_to_buffer(cstr filename, std::vector<u8>& buffer)
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

    buffer.resize((usize)fileSize);
    f.read((pstr)buffer.data(), fileSize);
    f.close();

    return true;
}

bool FileOpener::read_file_to_buffer(cstr filename, std::string& buffer)
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

    buffer.resize((usize) fileSize);
    f.read(&(buffer[0]), fileSize);
    f.close();

    return true;
}

bool FileOpener::read_file_lines(cstr filename, std::vector<std::string>& lines)
{
    std::ifstream f(filename);
    if (f.fail()) {
        return false;
    }
    std::string line;
    while (std::getline(f, line)) lines.push_back(line);
    return true;
}

bool FileOpener::open_file(cstr filename)
{
    file.clear();
    file.close();
    std::ifstream f(filename, std::ios::binary);
    file.open(filename, std::ios::binary);

    if (file.fail()) return false;
    return true;
}

bool FileOpener::open_File_lines(cstr filename)
{
    file.clear();
    file.close();
    std::ifstream f(filename);
    file.open(filename);

    if (file.fail()) return false;
    return true;
}

bool FileOpener::read_line(std::string& l)
{
    if(std::getline(file, l)) return true;
    return false;
}

void FileOpener::close_file() { file.close(); }

u32 FileOpener::get_size() { file.seekg(0, std::ios::end); return (u32) file.tellg(); }


u8 FileOpener::read_u8(i32 offset)
{
    file.seekg(offset);
    u8 result = 0;
    file.read(reinterpret_cast<pstr>(&result), 1);
    return result;
}

i8 FileOpener::read_i8(i32 offset)
{
    file.seekg(offset);
    i8 result = 0;
    file.read(reinterpret_cast<pstr>(&result), 1);
    return result;
}

u16 FileOpener::read_u16(i32 offset)
{
    file.seekg(offset);
    u16 result = 0;
    file.read(reinterpret_cast<pstr>(&result), 2);
    return result;
}

i16 FileOpener::read_i16(i32 offset)
{
    file.seekg(offset);
    i16 result = 0;
    file.read(reinterpret_cast<pstr>(&result), 2);
    return result;
}

u32 FileOpener::read_u32(i32 offset)
{
    file.seekg(offset);
    u32 result = 0;
    file.read(reinterpret_cast<pstr>(&result), 4);
    return result;
}

i32 FileOpener::read_i32(i32 offset)
{
    file.seekg(offset);
    i32 result = 0;
    file.read(reinterpret_cast<pstr>(&result), 4);
    return result;
}

f32 FileOpener::read_f32(i32 offset)
{
    union chartofloat {
        char in[4];
        f32 out;
    };
    file.seekg(offset);
    chartofloat result;
    result.out = 0.f;
    file.read(result.in, 4);
    return result.out;
}

std::string FileOpener::read_string(i32 offset, i32& offsetEnd)
{
    offsetEnd = offset;
    std::string thestring = {};
    char c = FileOpener::read_i8(offsetEnd);
    while (c != '\0')
    {
        thestring += c;
        offsetEnd++;
        c = FileOpener::read_i8(offsetEnd);
    }
    return thestring;
}

}
