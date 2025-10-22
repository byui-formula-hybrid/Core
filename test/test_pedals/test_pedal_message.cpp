#include <unity.h>
#include <cstring>
#include "../../lib/pedals/message.h"

// using namespace Pedals;

struct Message {
    unsigned char accelerator_percentage: 7;
    bool is_braking: 1;
    unsigned short accelerator_potentiometer1: 12;
    unsigned short accelerator_potentiometer2: 12;
    unsigned short brake_potentiometer1: 12;
    unsigned short brake_potentiometer2: 12;
    unsigned char brake_status: 3;
    unsigned char accelerator_status: 3;
    unsigned char reserved: 2;
};

enum class Status: unsigned short {
    OK = 0,
    HARDWARE_FAILURE = 1,
    SLEW_WARNING = 2,
    SLEW_CRITICAL = 3,
    CALIBRATION_WARNING = 4,
    CALIBRATION_CRITICAL = 5
};


void test_pedal_decoding() {
    uint8_t raw_buffer[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t* raw_buffer_ptr = raw_buffer;
    auto* message = reinterpret_cast<Message*>(raw_buffer_ptr);

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
    Message message;
    /*   
    * 0111 111       - 7  bits accelerator_percentage
    * 1              - 1  bit is_braking
    * 1010 1111 0111 - 12 bits accelerator_potentiometer1
    * 1111 1111 0000 - 12 bits accelerator_potentiometer2
    * 0101 1001 1111 - 12 bits brake_potentiometer1
    * 1000 0111 1100 - 12 bits brake_potentiometer2
    * 0010           - 4  bits brake_status
    * 0010           - 4  bits accelerator_status
    */
    uint8_t raw_buffer[8] = { 0x7F, 0xAF, 0x7F, 0xF0, 0x59, 0xF8, 0x7C, 0x22 };
    uint8_t* raw_buffer_ptr = raw_buffer;
    std::memcpy(&message, raw_buffer_ptr, sizeof(Message));

    TEST_ASSERT_EQUAL(63, message.accelerator_percentage);
    TEST_ASSERT_EQUAL(true, message.is_braking);
    TEST_ASSERT_EQUAL(2807, message.accelerator_potentiometer1);
    TEST_ASSERT_EQUAL(4080, message.accelerator_potentiometer2);
    TEST_ASSERT_EQUAL(1439, message.brake_potentiometer1);
    TEST_ASSERT_EQUAL(2172, message.brake_potentiometer2);
    TEST_ASSERT_EQUAL(2, message.brake_status);
    TEST_ASSERT_EQUAL(2, message.accelerator_status);
}

void test_pedal_binary_encoding() {
    Message message; 
    uint8_t raw_buffer[8] = {};
    uint8_t* raw_buffer_ptr = raw_buffer;
    std::memcpy(raw_buffer_ptr, &message, sizeof(Message));

    TEST_ASSERT_EQUAL(0, raw_buffer[0]);
    TEST_ASSERT_EQUAL(0, raw_buffer[1]);
    TEST_ASSERT_EQUAL(0, raw_buffer[2]);
    TEST_ASSERT_EQUAL(0, raw_buffer[3]);
    TEST_ASSERT_EQUAL(0, raw_buffer[4]);
    TEST_ASSERT_EQUAL(0, raw_buffer[5]);
    TEST_ASSERT_EQUAL(0, raw_buffer[6]);
    TEST_ASSERT_EQUAL(0, raw_buffer[7]);
}

void test_pedal_complex_binary_encoding() {
    Message message;
    message.accelerator_percentage = 57;
    message.is_braking = false;
    message.accelerator_potentiometer1 = 0;
    message.accelerator_potentiometer2 = 4095;
    message.brake_potentiometer1 = 2375;
    message.brake_potentiometer2 = 3749;
    message.brake_status = 0;
    message.accelerator_status = 4;

    uint8_t raw_buffer[8] = {};
    uint8_t* raw_buffer_ptr = raw_buffer;
    std::memcpy(raw_buffer_ptr, &message, sizeof(Message));

    TEST_ASSERT_EQUAL(0x39, raw_buffer[0]);
    TEST_ASSERT_EQUAL(0x00, raw_buffer[1]);
    TEST_ASSERT_EQUAL(0xD2, raw_buffer[2]);
    TEST_ASSERT_EQUAL(0xFF, raw_buffer[3]);
    TEST_ASSERT_EQUAL(0x57, raw_buffer[4]);
    TEST_ASSERT_EQUAL(0xAB, raw_buffer[5]);
    TEST_ASSERT_EQUAL(0x00, raw_buffer[6]);
    TEST_ASSERT_EQUAL(0x40, raw_buffer[7]);
}

void run_pedal_message_tests() {
    RUN_TEST(test_pedal_decoding);
    RUN_TEST(test_pedal_complex_binary_decoding);
    RUN_TEST(test_pedal_binary_encoding);
    RUN_TEST(test_pedal_complex_binary_encoding);
}
