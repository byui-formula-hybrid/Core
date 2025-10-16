#pragma once

#include "decode/i_decodable.h"
#include "encode/i_encodable.h"

namespace Core {

class iCodable: public iDecodable, public iEncodable {
  public: 
    iCodable() = default;

    virtual ~iCodable() = default;
};

} // namespace Core