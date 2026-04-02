#ifndef MOCK_CAN_SERVICE_H
#define MOCK_CAN_SERVICE_H

#include <can.h>

using namespace CAN;
using namespace std;

namespace MOCKS {

class MockCanService : public Service {
public:
    bool setup(const void * config) = 0;
    bool tear_down() = 0;
    void stop_listening() = 0;
    void start_listening() = 0;
    bool send(Frame* frame) = 0;
    const Frame* read() = 0;
    bool recover() = 0;

    ~Service() = default;
};

}  // namespace MOCKS

#endif // MOCK_CAN_SERVICE_H