#ifndef BYTESTREAMS_INCLUDED_H
#define BYTESTREAMS_INCLUDED_H

#include <vector>
#include <iostream>

namespace NSEngine {

    /** list of bytes ( std::vector<uint8_t> )*/
    typedef std::vector<uint8_t> Bytestream;

    /** offset in a Bytestream */
    enum class Offset : uint32_t {};

    namespace ConversionUnions {

        /** use conversion unions only in library's functions */
        union uint8ToUint8 {
            uint8_t in[1];
            uint8_t out;
        };

        /** use conversion unions only in library's functions */
        union uint8ToInt8 {
            uint8_t in[1];
            int8_t out;
        };

        /** use conversion unions only in library's functions */
        union uint8ToUint16 {
            uint8_t in[2];
            uint16_t out;
        };

        /** use conversion unions only in library's functions */
        union uint8ToInt16 {
            uint8_t in[2];
            int16_t out;
        };

        /** use conversion unions only in library's functions */
        union uint8ToUint32 {
            uint8_t in[4];
            uint32_t out;
        };

        /** use conversion unions only in library's functions */
        union uint8ToInt32 {
            uint8_t in[4];
            int32_t out;
        };

        /** use conversion unions only in library's functions */
        union uint8ToUint64 {
            uint8_t in[8];
            uint64_t out;
        };

        /** use conversion unions only in library's functions */
        union uint8ToInt64 {
            uint8_t in[8];
            int64_t out;
        };

        /** use conversion unions only in library's functions */
        union uint8ToFloat {
            uint8_t in[4];
            float out;
        };

        /** use conversion unions only in library's functions */
        union uint8ToDouble {
            uint8_t in[8];
            double out;
        };

    }

    /** reads n bytes from Bytestream bs from offset o and put it in uint8_t* out */
    inline void readBytestream(Bytestream const& bs, uint8_t* out, Offset o, size_t n)
    {
        for (size_t i = 0; i < n; i++)
            out[i] = bs[i+(size_t)o];
    }

    /** inserts n bytes in Bytestream bs from uint8_t* val */
    inline void insertBytestreamBytes(Bytestream& bs, uint8_t* val, size_t n)
    {
        for (size_t i = 0; i < n; i++)
            bs.push_back(val[i]);
    }

    /** inserts bytes of val in Bytestream bs */
    inline void insertBytestream(Bytestream& bs, uint8_t val)
    {
        ConversionUnions::uint8ToUint8 result;
        result.out = val;
        insertBytestreamBytes(bs, result.in, 1);
    }

    /** inserts bytes of val in Bytestream bs */
    inline void insertBytestream(Bytestream& bs, int8_t val)
    {
        ConversionUnions::uint8ToInt8 result;
        result.out = val;
        insertBytestreamBytes(bs, result.in, 1);
    }

    /** inserts bytes of val in Bytestream bs */
    inline void insertBytestream(Bytestream& bs, uint16_t val)
    {
        ConversionUnions::uint8ToUint16 result;
        result.out = val;
        insertBytestreamBytes(bs, result.in, 2);
    }

    /** inserts bytes of val in Bytestream bs */
    inline void insertBytestream(Bytestream& bs, int16_t val)
    {
        ConversionUnions::uint8ToInt16 result;
        result.out = val;
        insertBytestreamBytes(bs, result.in, 2);
    }

    /** inserts bytes of val in Bytestream bs */
    inline void insertBytestream(Bytestream& bs, uint32_t val)
    {
        ConversionUnions::uint8ToUint32 result;
        result.out = val;
        insertBytestreamBytes(bs, result.in, 4);
    }

    /** inserts bytes of val in Bytestream bs */
    inline void insertBytestream(Bytestream& bs, int32_t val)
    {
        ConversionUnions::uint8ToInt32 result;
        result.out = val;
        insertBytestreamBytes(bs, result.in, 4);
    }

    /** inserts bytes of val in Bytestream bs */
    inline void insertBytestream(Bytestream& bs, uint64_t val)
    {
        ConversionUnions::uint8ToUint64 result;
        result.out = val;
        insertBytestreamBytes(bs, result.in, 8);
    }

    /** inserts bytes of val in Bytestream bs */
    inline void insertBytestream(Bytestream& bs, int64_t val)
    {
        ConversionUnions::uint8ToInt64 result;
        result.out = val;
        insertBytestreamBytes(bs, result.in, 8);
    }

    /** inserts bytes of val in Bytestream bs */
    inline void insertBytestream(Bytestream& bs, float val)
    {
        ConversionUnions::uint8ToFloat result;
        result.out = val;
        insertBytestreamBytes(bs, result.in, 4);
    }

    /** inserts bytes of val in Bytestream bs */
    inline void insertBytestream(Bytestream& bs, double val)
    {
        ConversionUnions::uint8ToDouble result;
        result.out = val;
        insertBytestreamBytes(bs, result.in, 8);
    }

    template<typename T>
    Bytestream& operator<<(Bytestream& l, T const& r) {
        insertBytestream(l, r);
        return l;
    }

    /** reads an uint8_t at offset o in Bytestream bs */
    inline uint8_t getUint8(Bytestream const& bs, Offset o)
    {
        ConversionUnions::uint8ToUint8 result;
        readBytestream(bs, result.in, o, 1);
        return result.out;
    }

    /** reads an int8_t at offset o in Bytestream bs */
    inline int8_t getInt8(Bytestream const& bs, Offset o)
    {
        ConversionUnions::uint8ToInt8 result;
        readBytestream(bs, result.in, o, 1);
        return result.out;
    }

    /** reads an uint16_t at offset o in Bytestream bs */
    inline uint16_t getUint16(Bytestream const& bs, Offset o)
    {
        ConversionUnions::uint8ToUint16 result;
        readBytestream(bs, result.in, o, 2);
        return result.out;
    }

    /** reads an int16_t at offset o in Bytestream bs */
    inline int16_t getInt16(Bytestream const& bs, Offset o)
    {
        ConversionUnions::uint8ToInt16 result;
        readBytestream(bs, result.in, o, 2);
        return result.out;
    }

    /** reads an uint32_t at offset o in Bytestream bs */
    inline uint32_t getUint32(Bytestream const& bs, Offset o)
    {
        ConversionUnions::uint8ToUint32 result;
        readBytestream(bs, result.in, o, 4);
        return result.out;
    }

    /** reads an int32_t at offset o in Bytestream bs */
    inline int32_t getInt32(Bytestream const& bs, Offset o)
    {
        ConversionUnions::uint8ToInt32 result;
        readBytestream(bs, result.in, o, 4);
        return result.out;
    }

    /** reads an uint64_t at offset o in Bytestream bs */
    inline uint64_t getUint64(Bytestream const& bs, Offset o)
    {
        ConversionUnions::uint8ToUint64 result;
        readBytestream(bs, result.in, o, 8);
        return result.out;
    }

    /** reads an int64_t at offset o in Bytestream bs */
    inline int64_t getInt64(Bytestream const& bs, Offset o)
    {
        ConversionUnions::uint8ToInt64 result;
        readBytestream(bs, result.in, o, 8);
        return result.out;
    }

    /** reads a float at offset o in Bytestream bs */
    inline float getFloat(Bytestream const& bs, Offset o)
    {
        ConversionUnions::uint8ToFloat result;
        readBytestream(bs, result.in, o, 4);
        return result.out;
    }

    /** reads a double at offset o in Bytestream bs */
    inline double getDouble(Bytestream const& bs, Offset o)
    {
        ConversionUnions::uint8ToDouble result;
        readBytestream(bs, result.in, o, 8);
        return result.out;
    }

}

#endif
