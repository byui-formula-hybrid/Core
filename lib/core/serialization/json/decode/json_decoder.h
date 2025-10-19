#pragma once

#include "../../decode/i_decoder.h"

namespace Core {

class JSONDecoder: public iDecoder {
public:
    JSONDecoder() = default;

    iKeyedDecodingContainer* keyedContainer();
    iUnkeyedDecodingContainer* unkeyedContainer();

    ~JSONDecoder() = default;
};

} // namespace Core