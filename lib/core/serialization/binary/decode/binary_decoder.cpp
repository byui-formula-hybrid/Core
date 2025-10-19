#include "binary_decoder.h"

namespace Core {

iUnkeyedDecodingContainer* BinaryDecoder::unkeyedContainer() {
    return nullptr;
}

iKeyedDecodingContainer<int>* BinaryDecoder::keyedContainer() {
    return nullptr;
}

} // namespace Core