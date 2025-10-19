#pragma once

#include <cstddef>
#include <cstdint>
#include "decode_error.h"

namespace Core {

class iKeyedDecodingContainer {
public: 
    virtual const size_t allKeys(const char* buffer, size_t max_keys) = 0;
    virtual const bool contains(const char* key) = 0;

    virtual const DecodeError decodeBool(bool& out, const char* key) = 0;
    virtual const DecodeError decodeString(char* out, const char* key) = 0;
    virtual const DecodeError decodeUInt8(uint8_t& out, const char* key) = 0;
    virtual const DecodeError decodeUInt16(uint16_t& out, const char* key) = 0;
    virtual const DecodeError decodeUInt32(uint32_t& out, const char* key) = 0;
    virtual const DecodeError decodeUInt64(uint64_t& out, const char* key) = 0;

    ~iKeyedDecodingContainer() = default;
};

} // namespace Core