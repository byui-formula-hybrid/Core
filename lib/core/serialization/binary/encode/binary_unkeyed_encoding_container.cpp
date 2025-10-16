
#include <cstddef>
#include "binary_unkeyed_encoding_container.h"

// TODO: Implement bit packing for all encoding functions to save space

namespace Core {

BinaryUnkeyedEncodingContainer::BinaryUnkeyedEncodingContainer() : bufferIndex(0) {
  for (size_t i = 0; i < sizeof(buffer); ++i) {
    buffer[i] = 0;
  }
}

const uint8_t BinaryUnkeyedEncodingContainer::count() const { 
  return sizeof(buffer);
}

const bool BinaryUnkeyedEncodingContainer::isAtEnd() const { 
  return bufferIndex >= sizeof(buffer);
}

const uint8_t BinaryUnkeyedEncodingContainer::currentIndex() const { 
  return bufferIndex;
}

const uint8_t* BinaryUnkeyedEncodingContainer::data() const { 
  return buffer;
}

const EncodeError BinaryUnkeyedEncodingContainer::encodeBool(const bool in) { 
  if (isAtEnd()) {
    return EncodeError::EndOfContainer;
  }
  buffer[bufferIndex++] = in ? 1 : 0;
  return EncodeError::None;
}

const EncodeError BinaryUnkeyedEncodingContainer::encodeUInt8(const uint8_t in) {
  if (isAtEnd()) {
    return EncodeError::EndOfContainer;
  }
  buffer[bufferIndex++] = in;
  return EncodeError::None;
}

const EncodeError BinaryUnkeyedEncodingContainer::encodeUInt16(const uint16_t in) {
  if (isAtEnd()) {
    return EncodeError::EndOfContainer;
  }
  buffer[bufferIndex++] = in & 0xFF;
  buffer[bufferIndex++] = (in >> 8) & 0xFF;
  return EncodeError::None;
}

const EncodeError BinaryUnkeyedEncodingContainer::encodeUInt32(const uint32_t in) {
  if (isAtEnd()) {
    return EncodeError::EndOfContainer;
  }
  buffer[bufferIndex++] = in & 0xFF;
  buffer[bufferIndex++] = (in >> 8) & 0xFF;
  buffer[bufferIndex++] = (in >> 16) & 0xFF;
  buffer[bufferIndex++] = (in >> 24) & 0xFF;
  return EncodeError::None;
}

const EncodeError BinaryUnkeyedEncodingContainer::encodeUInt64(const uint64_t in) {
  if (isAtEnd()) {
    return EncodeError::EndOfContainer;
  }
  buffer[bufferIndex++] = in & 0xFF;
  buffer[bufferIndex++] = (in >> 8) & 0xFF;
  buffer[bufferIndex++] = (in >> 16) & 0xFF;
  buffer[bufferIndex++] = (in >> 24) & 0xFF;
  buffer[bufferIndex++] = (in >> 32) & 0xFF;
  buffer[bufferIndex++] = (in >> 40) & 0xFF;
  buffer[bufferIndex++] = (in >> 48) & 0xFF;
  buffer[bufferIndex++] = (in >> 56) & 0xFF;
  return EncodeError::None;
}

const EncodeError BinaryUnkeyedEncodingContainer::encodeString(const char* in) { 
  return EncodeError::NotSupported;
}

} // namespace Core
