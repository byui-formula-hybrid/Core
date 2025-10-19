#pragma once

#include "i_decoder.h"
#include "decode_error.h"

namespace Core {

class iDecodable {
public:
    virtual DecodeError decode(iDecoder& decoder) = 0;
    
    ~iDecodable() = default;
};

} // namespace Core