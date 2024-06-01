#ifndef BYTESTREAMS_INCLUDED_H
#define BYTESTREAMS_INCLUDED_H

#include <vector>
#include "./defines.h"

namespace ns {

/** list of bytes ( std::vector<u8> )*/
using Bytestream = std::vector<u8>;

/** offset in a Bytestream */
enum class Offset : u32 {};

namespace ConversionUnions {

/** use conversion unions only in library's functions */
union uint8ToUint8 {
  u8 in[1];
  u8 out;
};

/** use conversion unions only in library's functions */
union uint8ToInt8 {
  u8 in[1];
  i8 out;
};

/** use conversion unions only in library's functions */
union uint8ToUint16 {
  u8 in[2];
  u16 out;
};

/** use conversion unions only in library's functions */
union uint8ToInt16 {
  u8 in[2];
  i16 out;
};

/** use conversion unions only in library's functions */
union uint8ToUint32 {
  u8 in[4];
  u32 out;
};

/** use conversion unions only in library's functions */
union uint8ToInt32 {
  u8 in[4];
  i32 out;
};

/** use conversion unions only in library's functions */
union uint8ToUint64 {
  u8 in[8];
  u64 out;
};

/** use conversion unions only in library's functions */
union uint8ToInt64 {
  u8 in[8];
  i64 out;
};

/** use conversion unions only in library's functions */
union uint8ToFloat {
  u8 in[4];
  f32 out;
};

/** use conversion unions only in library's functions */
union uint8ToDouble {
  u8 in[8];
  f64 out;
};

} // namespace ConversionUnions

/** reads n bytes from Bytestream bs from offset o and put it in u8* out */
inline void readBytestream(Bytestream const &bs, u8 *out, Offset o,
                           usize n) {
  for (usize i = 0; i < n; i++)
    out[i] = bs[i + static_cast<usize>(o)];
}

/** inserts n bytes in Bytestream bs from u8* val */
inline void insertBytestreamBytes(Bytestream &bs, u8 *val, usize n) {
  for (usize i = 0; i < n; i++)
    bs.push_back(val[i]);
}

/** inserts bytes of val in Bytestream bs */
inline void insertBytestream(Bytestream &bs, u8 val) {
  ConversionUnions::uint8ToUint8 result;
  result.out = val;
  insertBytestreamBytes(bs, result.in, 1);
}

/** inserts bytes of val in Bytestream bs */
inline void insertBytestream(Bytestream &bs, i8 val) {
  ConversionUnions::uint8ToInt8 result;
  result.out = val;
  insertBytestreamBytes(bs, result.in, 1);
}

/** inserts bytes of val in Bytestream bs */
inline void insertBytestream(Bytestream &bs, u16 val) {
  ConversionUnions::uint8ToUint16 result;
  result.out = val;
  insertBytestreamBytes(bs, result.in, 2);
}

/** inserts bytes of val in Bytestream bs */
inline void insertBytestream(Bytestream &bs, i16 val) {
  ConversionUnions::uint8ToInt16 result;
  result.out = val;
  insertBytestreamBytes(bs, result.in, 2);
}

/** inserts bytes of val in Bytestream bs */
inline void insertBytestream(Bytestream &bs, u32 val) {
  ConversionUnions::uint8ToUint32 result;
  result.out = val;
  insertBytestreamBytes(bs, result.in, 4);
}

/** inserts bytes of val in Bytestream bs */
inline void insertBytestream(Bytestream &bs, i32 val) {
  ConversionUnions::uint8ToInt32 result;
  result.out = val;
  insertBytestreamBytes(bs, result.in, 4);
}

/** inserts bytes of val in Bytestream bs */
inline void insertBytestream(Bytestream &bs, u64 val) {
  ConversionUnions::uint8ToUint64 result;
  result.out = val;
  insertBytestreamBytes(bs, result.in, 8);
}

/** inserts bytes of val in Bytestream bs */
inline void insertBytestream(Bytestream &bs, i64 val) {
  ConversionUnions::uint8ToInt64 result;
  result.out = val;
  insertBytestreamBytes(bs, result.in, 8);
}

/** inserts bytes of val in Bytestream bs */
inline void insertBytestream(Bytestream &bs, f32 val) {
  ConversionUnions::uint8ToFloat result;
  result.out = val;
  insertBytestreamBytes(bs, result.in, 4);
}

/** inserts bytes of val in Bytestream bs */
inline void insertBytestream(Bytestream &bs, f64 val) {
  ConversionUnions::uint8ToDouble result;
  result.out = val;
  insertBytestreamBytes(bs, result.in, 8);
}

template <typename T> Bytestream &operator<<(Bytestream &l, T const &r) {
  insertBytestream(l, r);
  return l;
}

/** reads an u8 at offset o in Bytestream bs */
inline u8 getUint8(Bytestream const &bs, Offset o) {
  ConversionUnions::uint8ToUint8 result;
  readBytestream(bs, result.in, o, 1);
  return result.out;
}

/** reads an i8 at offset o in Bytestream bs */
inline i8 getInt8(Bytestream const &bs, Offset o) {
  ConversionUnions::uint8ToInt8 result;
  readBytestream(bs, result.in, o, 1);
  return result.out;
}

/** reads an u16 at offset o in Bytestream bs */
inline u16 getUint16(Bytestream const &bs, Offset o) {
  ConversionUnions::uint8ToUint16 result;
  readBytestream(bs, result.in, o, 2);
  return result.out;
}

/** reads an i16 at offset o in Bytestream bs */
inline i16 getInt16(Bytestream const &bs, Offset o) {
  ConversionUnions::uint8ToInt16 result;
  readBytestream(bs, result.in, o, 2);
  return result.out;
}

/** reads an u32 at offset o in Bytestream bs */
inline u32 getUint32(Bytestream const &bs, Offset o) {
  ConversionUnions::uint8ToUint32 result;
  readBytestream(bs, result.in, o, 4);
  return result.out;
}

/** reads an i32 at offset o in Bytestream bs */
inline i32 getInt32(Bytestream const &bs, Offset o) {
  ConversionUnions::uint8ToInt32 result;
  readBytestream(bs, result.in, o, 4);
  return result.out;
}

/** reads an u64 at offset o in Bytestream bs */
inline u64 getUint64(Bytestream const &bs, Offset o) {
  ConversionUnions::uint8ToUint64 result;
  readBytestream(bs, result.in, o, 8);
  return result.out;
}

/** reads an i64 at offset o in Bytestream bs */
inline i64 getInt64(Bytestream const &bs, Offset o) {
  ConversionUnions::uint8ToInt64 result;
  readBytestream(bs, result.in, o, 8);
  return result.out;
}

/** reads a f32 at offset o in Bytestream bs */
inline f32 getFloat(Bytestream const &bs, Offset o) {
  ConversionUnions::uint8ToFloat result;
  readBytestream(bs, result.in, o, 4);
  return result.out;
}

/** reads a f64 at offset o in Bytestream bs */
inline f64 getDouble(Bytestream const &bs, Offset o) {
  ConversionUnions::uint8ToDouble result;
  readBytestream(bs, result.in, o, 8);
  return result.out;
}

} // namespace NSEngine

#endif
