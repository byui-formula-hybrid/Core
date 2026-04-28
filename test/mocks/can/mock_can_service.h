#ifndef MOCK_CAN_SERVICE_H
#define MOCK_CAN_SERVICE_H

#include <functional>

#include <can.h>

using namespace CAN;

namespace MOCKS {

class MockCanService : public CAN::Service {
public:
    std::function<bool(const Frame& frame)> on_send = [](const Frame&){ return true; };

    struct CallCount {
        int setup = 0;
        int tear_down = 0;
        int stop_listening = 0;
        int start_listening = 0;
        int send = 0;
        int read = 0;
        int recover = 0;
    } calls;

    bool setup(const void* config) override {
        calls.setup++;
        return true;
    }

    bool tear_down() override {
        calls.tear_down++;
        return true;
    }

    void stop_listening() override {
        calls.stop_listening++;
    }
    void start_listening() override {
        calls.start_listening++;
    }
    bool send(const Frame& frame) override {
        calls.send++;
        return on_send(frame);
    }
    const Frame* read() override {
        calls.read++;
        return nullptr;
    }
    bool recover() override {
        calls.recover++;
        return true;
    }

    MockCanService() = default;
    ~MockCanService() = default;
};

}  // namespace MOCKS

#endif // MOCK_CAN_SERVICE_H