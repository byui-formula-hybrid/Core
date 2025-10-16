#pragma once

#include "i_keyed_decoding_container.h"
#include "i_unkeyed_decoding_container.h"

namespace Core {

template<typename Key> class iKeyedDecodingContainer;
class iUnkeyedDecodingContainer;

class iDecoder {
  public: 
    virtual iUnkeyedDecodingContainer* unkeyedContainer() = 0;
    virtual iKeyedDecodingContainer* keyedContainer() = 0;

    ~iDecoder() = default;
};

} // namespace Core