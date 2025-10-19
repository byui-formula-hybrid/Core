#pragma once

#include "../../decode/i_unkeyed_decoding_container.h"

namespace Core {

class BinaryUnkeyedDecodingContainer: public iUnkeyedDecodingContainer {
public:
    const uint8_t count() const;
    const bool isAtEnd() const;
    const uint8_t currentIndex() const;
    const DecodeError decodeBool(bool& out);
    const DecodeError decodeUInt8(uint8_t& out);
    const DecodeError decodeUInt16(uint16_t& out);
    const DecodeError decodeUInt32(uint32_t& out);
    const DecodeError decodeUInt64(uint64_t& out);
    const DecodeError decodeString(char* out);

    ~BinaryUnkeyedDecodingContainer() = default;

private:
    // Private members can be added here in the future if needed
};

} // namespace Core
