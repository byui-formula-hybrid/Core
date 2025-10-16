#pragma once

#include "../../decode/i_decoder.h"

namespace Core {

class BinaryDecoder: public iDecoder {
  public:
    BinaryDecoder() = default;

    virtual ~BinaryDecoder() = default;
};

} // namespace Core