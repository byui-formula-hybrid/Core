#include <unity.h>
#include <cstring>
#include "../../lib/pedals/message.h"
#include "../../lib/can/service.h"

using namespace Pedals;
using namespace CAN;

void test_pedal_decoding() {
    Frame frame;
    frame.identifier = 0x123;
    frame.data_length_code = 8;
    frame.data[0] = 0x00;
    frame.data[1] = 0x00;
    frame.data[2] = 0x00;
    frame.data[3] = 0x00;
    frame.data[4] = 0x00;
    frame.data[5] = 0x00;
    frame.data[6] = 0x00;
    frame.data[7] = 0x00;

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
    Frame frame;
    frame.identifier = 0x123;
    frame.data_length_code = 8;

    /*   
    * 0011 111       - 7  bits accelerator_percentage
    * 1              - 1  bit is_braking
    * 1010 1111 0111 - 12 bits accelerator_potentiometer1
    * 1111 1111 0000 - 12 bits accelerator_potentiometer2
    * 0101 1001 1111 - 12 bits brake_potentiometer1
    * 1000 0111 1100 - 12 bits brake_potentiometer2
    * 0010           - 4  bits brake_status
    * 0010           - 4  bits accelerator_status
    */
    frame.data[0] = 0xBF;
    frame.data[1] = 0xF7;
    frame.data[2] = 0x0A;
    frame.data[3] = 0xFF;
    frame.data[4] = 0x9F;
    frame.data[5] = 0xC5;
    frame.data[6] = 0x87;
    frame.data[7] = 0x12;
    
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
