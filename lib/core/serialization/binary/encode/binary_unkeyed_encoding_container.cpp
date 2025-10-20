#include <cstddef>
#include "binary_unkeyed_encoding_container.h"

// TODO: Implement bit packing for all encoding functions to save space

namespace Core {

const uint8_t BinaryUnkeyedEncodingContainer::count() const { 
  return 0;
  // return buffer.size();
}

const bool BinaryUnkeyedEncodingContainer::isAtEnd() const { 
  return false;
  // return buffer.size() >= buffer.capacity();
}

const uint8_t BinaryUnkeyedEncodingContainer::currentIndex() const { 
  return 0;
  // return buffer.size();
}

const uint8_t* BinaryUnkeyedEncodingContainer::data() const { 
  return nullptr;
  // return buffer.bytes();
}

const EncodeError BinaryUnkeyedEncodingContainer::encodeBool(const bool in) { 
  if (isAtEnd()) {
    return EncodeError::EndOfContainer;
  }
  buffer.append(in ? 1 : 0);
  return EncodeError::None;
}

const EncodeError BinaryUnkeyedEncodingContainer::encodeUInt8(const uint8_t in) {
  if (isAtEnd()) {
    return EncodeError::EndOfContainer;
  }
  buffer.append(in);
  return EncodeError::None;
}

const EncodeError BinaryUnkeyedEncodingContainer::encodeUInt16(const uint16_t in) {
  if (isAtEnd()) {
    return EncodeError::EndOfContainer;
  }
  buffer.append(in & 0xFF);
  buffer.append((in >> 8) & 0xFF);
  return EncodeError::None;
}

const EncodeError BinaryUnkeyedEncodingContainer::encodeUInt32(const uint32_t in) {
  if (isAtEnd()) {
    return EncodeError::EndOfContainer;
  }
  buffer.append(in & 0xFF);
  buffer.append((in >> 8) & 0xFF);
  buffer.append((in >> 16) & 0xFF);
  buffer.append((in >> 24) & 0xFF);
  return EncodeError::None;
}

const EncodeError BinaryUnkeyedEncodingContainer::encodeUInt64(const uint64_t in) {
  if (isAtEnd()) {
    return EncodeError::EndOfContainer;
  }
  
  buffer.append(in & 0xFF);
  buffer.append((in >> 8) & 0xFF);
  buffer.append((in >> 16) & 0xFF);
  buffer.append((in >> 24) & 0xFF);
  buffer.append((in >> 32) & 0xFF);
  buffer.append((in >> 40) & 0xFF);
  buffer.append((in >> 48) & 0xFF);
  buffer.append((in >> 56) & 0xFF);
  return EncodeError::None;
}

const EncodeError BinaryUnkeyedEncodingContainer::encodeString(const char* in) { 
  return EncodeError::NotSupported;
}

const Data BinaryUnkeyedEncodingContainer::data() {
  return buffer;
}

} // namespace Core
