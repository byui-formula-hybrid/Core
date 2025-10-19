#pragma once

#include "../../decode/i_decoder.h"

namespace Core {

class BinaryDecoder: public iDecoder {
public:
    iUnkeyedDecodingContainer* unkeyedContainer();
    iKeyedDecodingContainer* keyedContainer();

    ~BinaryDecoder() = default;
};

} // namespace Core