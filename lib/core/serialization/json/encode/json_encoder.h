#pragma once

#include "../../encode/i_encoder.h"

namespace Core {

class JSONEncoder: public iEncoder {
public:
    JSONEncoder() = default;

    iKeyedEncodingContainer* keyedContainer();
    iUnkeyedEncodingContainer* unkeyedContainer();

    ~JSONEncoder() = default;
};

} // namespace Core
