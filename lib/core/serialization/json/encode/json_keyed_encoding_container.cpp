#include "json_keyed_encoding_container.h"

namespace Core {

const EncodeError JSONKeyedEncodingContainer::encodeBool(bool value, const char* key) {
    return EncodeError::None;
}

const EncodeError JSONKeyedEncodingContainer::encodeString(const char* value, const char* key) {
  	return EncodeError::None;
}

const EncodeError JSONKeyedEncodingContainer::encodeUInt8(uint8_t value, const char* key) {
  	return EncodeError::None;
}

const EncodeError JSONKeyedEncodingContainer::encodeUInt16(uint16_t value, const char* key) {
  	return EncodeError::None;
}

const EncodeError JSONKeyedEncodingContainer::encodeUInt32(uint32_t value, const char* key) {
  	return EncodeError::None;
}

const EncodeError JSONKeyedEncodingContainer::encodeUInt64(uint64_t value, const char* key) {
  	return EncodeError::None;
}

} // namespace Core
