#pragma once

namespace Core {

class queue {
public:
    virtual void insert(void* data);
    virtual void remove(void* data);
};

} // namespace Core