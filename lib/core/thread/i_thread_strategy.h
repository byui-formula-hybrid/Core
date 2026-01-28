#pragma once

namespace Core {

typedef taskFunc void*()(void*)

class iThreadStrategy {
public:
    virtual void setup(const char* name, uint32_t priority, uint32_t attributes);
    virtual uint32_t create(taskFunc task, void* argument);
    virtual void join();
}

}