#pragma once

#include "i_unkeyed_encoding_container.h"
#include "i_keyed_encoding_container.h"

namespace Core {

class iEncoder {
public: 
    virtual iKeyedEncodingContainer* keyedContainer() = 0;
    virtual iUnkeyedEncodingContainer* unkeyedContainer() = 0;
    
    ~iEncoder() = default;
};

} // namespace Core