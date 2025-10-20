#pragma once

#include "../data.h"
#include "i_encoder.h"
#include "encode_error.h"

namespace Core {

class iEncodable {
public: 
    virtual EncodeError encode(iEncoder& encoder, Data& out) const = 0;
    
    ~iEncodable() = default;
};

} // namespace Core