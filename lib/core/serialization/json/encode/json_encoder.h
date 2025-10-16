#pragma once

#include "../../encode/i_encoder.h"

namespace Core {

class JSONEncoder: public iEncoder {
  public:
    JSONEncoder() = default;

    virtual ~JSONEncoder() = default;
};

} // namespace Core
