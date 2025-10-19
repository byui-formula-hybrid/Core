#pragma once

#include <cstdint>
#include "decode_error.h"

namespace Core {

class iUnkeyedDecodingContainer {
public: 
    virtual const uint8_t count() = 0;
    virtual const bool isAtEnd() = 0;
    virtual const uint8_t currentIndex() = 0;

    virtual const DecodeError decodeBool(bool& out) = 0;
    virtual const DecodeError decodeUInt8(uint8_t& out) = 0;
    virtual const DecodeError decodeUInt16(uint16_t& out) = 0;
    virtual const DecodeError decodeUInt32(uint32_t& out) = 0;
    virtual const DecodeError decodeUInt64(uint64_t& out) = 0;
    virtual const DecodeError decodeString(char* out) = 0;

    ~iUnkeyedDecodingContainer() = default;
};

} // namespace Core