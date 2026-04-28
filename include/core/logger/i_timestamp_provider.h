#ifndef I_TIME_STAMP_PROVIDER_H
#define I_TIME_STAMP_PROVIDER_H

#include <stdint.h>

class ITimeStampProvider {
public:
    virtual ~ITimeStampProvider() = default;
    virtual uint64_t get_timestamp() = 0;
};

#endif // I_TIME_STAMP_PROVIDER_H