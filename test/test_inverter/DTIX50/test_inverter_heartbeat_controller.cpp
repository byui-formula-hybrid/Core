#include <unity.h>

#include <memory>
#include <thread>
#include <chrono>

#include <DTIX50.h>
#include <mocks.h>

using namespace Inverter;
using namespace MOCKS;

Transmitter* canTransmitter = Transmitter::get_instance();

void transmit_loop(void *) {
    for(int i = 0; i < 1000; i++) {
        canTransmitter->transmit();
        // A Slight delay to replicate some real time delay
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void test_Heartbeat() {
    MockCanService* canService = new MockCanService(); // Most likely the ownership should be outside of the class
    int drive_type[2] = {0, 0};
    canService->on_send = [&drive_type](const Frame& frame){ 
        if(frame.data[0] == 1)
            drive_type[0]++; 
        else if (frame.data[0] == 0)
            drive_type[1]++;
        return true; 
    };

    NativeQueueStrategy<Frame>* queue = new NativeQueueStrategy<Frame>();

    canTransmitter->set_service(canService);
    canTransmitter->set_queue(queue);
    std::unique_ptr<Core::iLockStrategy> lockStrategy(new NativeLockStrategy()); // We'll want the class to recieve ownership
    std::unique_ptr<Core::iThreadStrategy> threadStrategy(new NativeThreadStrategy()); // We'll want the class to recieve ownership

    DTIX50::Heartbeat heartbeat(canTransmitter, std::move(lockStrategy), std::move(threadStrategy));
    
    std::thread transmit_thread = std::thread(transmit_loop, nullptr);

    TEST_ASSERT(!heartbeat.started());
    heartbeat.start();
    TEST_ASSERT(heartbeat.started());

    // We should always get at least 3 transmits
    std::this_thread::sleep_for(std::chrono::seconds(1));
    TEST_ASSERT_GREATER_OR_EQUAL(3, ((MockCanService*)canService)->calls.send);

    TEST_ASSERT_GREATER_OR_EQUAL(2, drive_type[0]); // Verify that we have sent at least 2 drive enables.

    heartbeat.stop();
    transmit_thread.join();

    TEST_ASSERT(!heartbeat.started());

    TEST_ASSERT_EQUAL(1, drive_type[1]); // Verify that only one drive disable has been sent

    free(canService);
}

void run_DTIX50_controller_tests() {
    RUN_TEST(test_Heartbeat);
}