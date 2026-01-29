#pragma once

typedef void(*taskFunc)(void*);

namespace Core {

class iThreadStrategy {
public:
    virtual ~iThreadStrategy() = default;
    virtual void setup(const char* name = nullptr, uint32_t priority = 0, uint32_t attributes = 0) = 0;
    virtual uint32_t create(taskFunc task, void* argument) = 0;
    virtual void join() = 0;
    virtual void sleep(uint32_t millis) = 0;
};

}