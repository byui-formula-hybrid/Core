#pragma once

#include "binary_unkeyed_decoding_container.h"

namespace Core {

const uint8_t BinaryUnkeyedDecodingContainer::count() const {
    return 0;
}

const bool BinaryUnkeyedDecodingContainer::isAtEnd() const {
    return true;
}

const uint8_t BinaryUnkeyedDecodingContainer::currentIndex() const {
    return 0;
}

const DecodeError BinaryUnkeyedDecodingContainer::decodeBool(bool& out) {
    return DecodeError::None;
}

const DecodeError BinaryUnkeyedDecodingContainer::decodeUInt8(uint8_t& out) {
    return DecodeError::None;
}

const DecodeError BinaryUnkeyedDecodingContainer::decodeUInt16(uint16_t& out) {
    return DecodeError::None;
}

const DecodeError BinaryUnkeyedDecodingContainer::decodeUInt32(uint32_t& out) {
    return DecodeError::None;
}

const DecodeError BinaryUnkeyedDecodingContainer::decodeUInt64(uint64_t& out) {
    return DecodeError::None;
}

const DecodeError BinaryUnkeyedDecodingContainer::decodeString(char* out) {
    return DecodeError::None;
}

} // namespace Core
