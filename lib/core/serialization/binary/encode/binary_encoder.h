#pragma once

#include "../../encode/i_encoder.h"
#include "binary_unkeyed_encoding_container.h"

namespace Core {

class BinaryEncoder: public iEncoder {
public:
    iKeyedEncodingContainer* keyedContainer();
    iUnkeyedEncodingContainer* unkeyedContainer();

private:
    BinaryUnkeyedEncodingContainer binaryUnkeyedContainer;
};

} // namespace Core
