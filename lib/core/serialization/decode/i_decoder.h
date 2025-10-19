#pragma once

#include "i_keyed_decoding_container.h"
#include "i_unkeyed_decoding_container.h"

namespace Core {

template<typename Key> class iKeyedDecodingContainer;
class iUnkeyedDecodingContainer;

class iDecoder {
public: 
    virtual iKeyedDecodingContainer* keyedContainer() = 0;
    virtual iUnkeyedDecodingContainer* unkeyedContainer() = 0;

    ~iDecoder() = default;
};

} // namespace Core