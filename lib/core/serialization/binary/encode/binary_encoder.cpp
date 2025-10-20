#include "binary_encoder.h"
#include "binary_unkeyed_encoding_container.h"

namespace Core {

BinaryEncoder::BinaryEncoder() : binaryUnkeyedContainer() {}

iKeyedEncodingContainer* BinaryEncoder::keyedContainer() {
    return nullptr;
}

iUnkeyedEncodingContainer* BinaryEncoder::unkeyedContainer() {
    return &binaryUnkeyedContainer;
}

} // namespace Core