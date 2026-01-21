#include <unity.h>
#include <cstring>
#include "../../lib/pedals/message.h"
#include "../../lib/can/service.h"

using namespace Pedals;
using namespace CAN;

void test_pedal_decoding() {
    uint8_t data[8] = {0,0,0,0,0,0,0,0};
    Frame frame = Frame(0x123, data);

    auto message = frame.decode<Message>();

    TEST_ASSERT_EQUAL(0, message->accelerator_percentage);
    TEST_ASSERT_EQUAL(false, message->is_braking);
    TEST_ASSERT_EQUAL(0, message->accelerator_potentiometer1);
    TEST_ASSERT_EQUAL(0, message->accelerator_potentiometer2);
    TEST_ASSERT_EQUAL(0, message->brake_potentiometer1);
    TEST_ASSERT_EQUAL(0, message->brake_potentiometer2);
    TEST_ASSERT_EQUAL(0, message->brake_status);
    TEST_ASSERT_EQUAL(0, message->accelerator_status);
}

void test_pedal_complex_binary_decoding() {
    uint8_t data[8] = { 0xBF, 0xF7, 0x0A, 0xFF, 0x9F, 0xC5, 0x87, 0x12 };
    Frame frame = Frame(0x123, data);
    
    auto message = frame.decode<Pedals::Message>();

    TEST_ASSERT_EQUAL(63, message->accelerator_percentage);
    TEST_ASSERT_EQUAL(true, message->is_braking);
    TEST_ASSERT_EQUAL(2807, message->accelerator_potentiometer1);
    TEST_ASSERT_EQUAL(4080, message->accelerator_potentiometer2);
    TEST_ASSERT_EQUAL(1439, message->brake_potentiometer1);
    TEST_ASSERT_EQUAL(2172, message->brake_potentiometer2);
    TEST_ASSERT_EQUAL(Status::SLEW_WARNING, message->brake_status);
    TEST_ASSERT_EQUAL(Status::SLEW_WARNING, message->accelerator_status);
}

void test_pedal_binary_encoding() {
    Pedals::Message message;

    message.accelerator_percentage = 0;
    message.is_braking = false;
    message.accelerator_potentiometer1 = 0;
    message.accelerator_potentiometer2 = 0;
    message.brake_potentiometer1 = 0;
    message.brake_potentiometer2 = 0;
    message.brake_status = Status::OK;
    message.accelerator_status = Status::OK;

    Frame frame(0, &message);

    TEST_ASSERT_EQUAL(0, frame.data[0]);
    TEST_ASSERT_EQUAL(0, frame.data[1]);
    TEST_ASSERT_EQUAL(0, frame.data[2]);
    TEST_ASSERT_EQUAL(0, frame.data[3]);
    TEST_ASSERT_EQUAL(0, frame.data[4]);
    TEST_ASSERT_EQUAL(0, frame.data[5]);
    TEST_ASSERT_EQUAL(0, frame.data[6]);
    TEST_ASSERT_EQUAL(0, frame.data[7]);
}

void test_pedal_complex_binary_encoding() {
    Pedals::Message message;
    message.accelerator_percentage = 0x39;
    message.is_braking = 0x0;
    message.accelerator_potentiometer1 = 0x0;
    message.accelerator_potentiometer2 = 0xFFF;
    message.brake_potentiometer1 = 0x947;
    message.brake_potentiometer2 = 0xEAD;
    message.brake_status = (Status)0x4;
    message.accelerator_status = (Status)0x2;

    Frame frame(0, &message);

    TEST_ASSERT_EQUAL(0x39, frame.data[0]);
    TEST_ASSERT_EQUAL(0x00, frame.data[1]);
    TEST_ASSERT_EQUAL(0xF0, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0x47, frame.data[4]);
    TEST_ASSERT_EQUAL(0xD9, frame.data[5]);
    TEST_ASSERT_EQUAL(0xEA, frame.data[6]);
    TEST_ASSERT_EQUAL(0x14, frame.data[7]);
}

void run_pedal_message_tests() {
    RUN_TEST(test_pedal_decoding);
    RUN_TEST(test_pedal_complex_binary_decoding);
    RUN_TEST(test_pedal_binary_encoding);
    RUN_TEST(test_pedal_complex_binary_encoding);
}
