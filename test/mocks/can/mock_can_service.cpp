#include "mock_can_service.h"

namespace MOCKS {

bool MockCanService::setup(const void * config) {
    return true;
}

bool MockCanService::tear_down() {
    return true;
}

void MockCanService::stop_listening() {
    
}

void MockCanService::start_listening() {

}

bool MockCanService::send(Frame* frame) {
    return true;
}

const Frame* MockCanService::read() {
    return nullptr;
}

bool MockCanService::recover() {
    return true;
}

}  // namespace MOCKS