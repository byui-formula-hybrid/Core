#pragma once

#include <cstdint>
#include "encode_error.h"

namespace Core {

class iUnkeyedEncodingContainer {
  public:
    virtual const uint8_t count() const = 0;
    virtual const bool isAtEnd() const = 0;
    virtual const uint8_t currentIndex() const = 0;

    virtual const EncodeError encodeBool(const bool value) = 0;
    virtual const EncodeError encodeString(const char* value) = 0;
    virtual const EncodeError encodeUInt8(const uint8_t value) = 0;
    virtual const EncodeError encodeUInt16(const uint16_t value) = 0;
    virtual const EncodeError encodeUInt32(const uint32_t value) = 0;
    virtual const EncodeError encodeUInt64(const uint64_t value) = 0;

    ~iUnkeyedEncodingContainer() = default;
};

} // namespace Core