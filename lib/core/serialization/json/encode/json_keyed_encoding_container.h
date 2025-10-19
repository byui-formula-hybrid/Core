#pragma once

#include "../../encode/i_keyed_encoding_container.h"

namespace Core {

class JSONKeyedEncodingContainer: public iKeyedEncodingContainer {
public:
    JSONKeyedEncodingContainer() = default;

    const EncodeError encodeBool(bool value, const char* key);
    const EncodeError encodeString(const char* value, const char* key);
    const EncodeError encodeUInt8(uint8_t value, const char* key);
    const EncodeError encodeUInt16(uint16_t value, const char* key);
    const EncodeError encodeUInt32(uint32_t value, const char* key);
    const EncodeError encodeUInt64(uint64_t value, const char* key);

private:
    // Private members for JSON parsing
};

} // namespace Core
