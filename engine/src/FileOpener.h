#ifndef FILEOPENER_HEADER_INCLUDED
#define FILEOPENER_HEADER_INCLUDED

#include <vector>
#include <string>
#include "./defines.h"

namespace ns {

class FileOpener
{
    public:
        // XXX: Never used Bytestream:
        // static bool Read(Bytestream& bs, cstr file);
        // static bool Write(Bytestream const& bs, cstr file);

        static bool read_file_to_buffer(cstr filename, ptr &data, i32& size);
        static bool read_file_to_buffer(cstr filename, std::vector<char>& buffer);
        static bool read_file_to_buffer(cstr filename, std::vector<u8>& buffer);
        static bool read_file_to_buffer(cstr filename, std::string& buffer);
        static bool read_file_lines(cstr filename, std::vector<std::string>& lines);

        static bool open_file(cstr filename);
        static bool open_File_lines(cstr filename);
        static void close_file();
        static u32 get_size();

        static bool read_line(std::string& l);
        static u8 read_u8(i32 offset);
        static i8 read_i8(i32 offset);
        static u16 read_u16(i32 offset);
        static i16 read_i16(i32 offset);
        static u32 read_u32(i32 offset);
        static i32 read_i32(i32 offset);
        static f32 read_f32(i32 offset);
    
        static std::string read_string(int offset, int& offset_end);
};

} // namespace ns

#endif // FILEOPENER_HEADER_INCLUDED
