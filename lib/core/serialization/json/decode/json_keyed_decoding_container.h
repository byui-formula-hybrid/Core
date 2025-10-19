#pragma once

#include "../../decode/i_keyed_decoding_container.h"

namespace Core {

class JSONKeyedDecodingContainer: public iKeyedDecodingContainer {
public:
    JSONKeyedDecodingContainer() = default;

    const DecodeError decodeBool(bool& out, const char* key);
    const DecodeError decodeString(char* out, const char* key);
    const DecodeError decodeUInt8(uint8_t& out, const char* key);
    const DecodeError decodeUInt16(uint16_t& out, const char* key);
    const DecodeError decodeUInt32(uint32_t& out, const char* key);
    const DecodeError decodeUInt64(uint64_t& out, const char* key);

private:
    // Private members for JSON parsing
};

} // namespace Core
