#pragma once

#include "../../decode/i_decoder.h"

namespace Core {

class JSONDecoder: public iDecoder {
  public:
    JSONDecoder() = default;

    virtual ~JSONDecoder() = default;
};

} // namespace Core