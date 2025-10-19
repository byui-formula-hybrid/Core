#include "json_keyed_decoding_container.h"

namespace Core {

const DecodeError JSONKeyedDecodingContainer::decodeBool(bool& out, const char* key) {
    return DecodeError::None;
}

const DecodeError JSONKeyedDecodingContainer::decodeString(char* out, const char* key) {
  	return DecodeError::None;
}

const DecodeError JSONKeyedDecodingContainer::decodeUInt8(uint8_t& out, const char* key) {
  	return DecodeError::None;
}

const DecodeError JSONKeyedDecodingContainer::decodeUInt16(uint16_t& out, const char* key) {
  	return DecodeError::None;
}

const DecodeError JSONKeyedDecodingContainer::decodeUInt32(uint32_t& out, const char* key) {
  	return DecodeError::None;
}

const DecodeError JSONKeyedDecodingContainer::decodeUInt64(uint64_t& out, const char* key) {
  	return DecodeError::None;
}

} // namespace Core
