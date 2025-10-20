#pragma once

#include <cstddef>
#include "../../encode/i_unkeyed_encoding_container.h"
#include "../../encode/i_encodable.h"
#include "../../data.h"

namespace Core {

class BinaryUnkeyedEncodingContainer: public iUnkeyedEncodingContainer {
public:
    BinaryUnkeyedEncodingContainer() : buffer(8) {}

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

    const Data data();

private:
    Data buffer;
};

} // namespace Core
