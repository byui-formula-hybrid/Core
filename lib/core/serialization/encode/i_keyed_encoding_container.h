#pragma once

#include <cstdint>
#include "encode_error.h"
#include "../data.h"

namespace Core {

class iKeyedEncodingContainer {
public: 
    virtual const EncodeError encodeBool(const bool value, const char* key) = 0;
    virtual const EncodeError encodeString(const char* value, const char* key) = 0;
    virtual const EncodeError encodeUInt8(const uint8_t value, const char* key) = 0;
    virtual const EncodeError encodeUInt16(const uint16_t value, const char* key) = 0;
    virtual const EncodeError encodeUInt32(const uint32_t value, const char* key) = 0;
    virtual const EncodeError encodeUInt64(const uint64_t value, const char* key) = 0;

    virtual const Data data() = 0;

    ~iKeyedEncodingContainer() = default;
};

} // namespace Core