#pragma once

#include <cstddef>
#include "../../encode/i_unkeyed_encoding_container.h"
#include "../../encode/i_encodable.h"

namespace Core {

class BinaryUnkeyedEncodingContainer: public iUnkeyedEncodingContainer {
  public:
    BinaryUnkeyedEncodingContainer();

    const uint8_t count() const;
    const bool isAtEnd() const;
    const uint8_t currentIndex() const;
    const uint8_t* data() const;
    const EncodeError encodeBool(const bool in);
    const EncodeError encodeUInt8(const uint8_t in);
    const EncodeError encodeUInt16(const uint16_t in);
    const EncodeError encodeUInt32(const uint32_t in);
    const EncodeError encodeUInt64(const uint64_t in);
    const EncodeError encodeString(const char* in);

  private:
    uint8_t buffer[8];
    uint8_t bufferIndex = 0;
};

} // namespace Core
