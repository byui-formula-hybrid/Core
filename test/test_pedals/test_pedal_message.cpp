#include "unity.h"
#include "../../lib/pedals/pedal_message.h"
#include "../../lib/core/serialization/binary/encode/binary_encoder.h"

using namespace Pedals;
using namespace Core;

// TODO: What does the encoded object look like? Is it some kind of data object?
void test_pedal_binary_encoding() {
    PedalMessage message;
    BinaryEncoder encoder;
    Data out(8);
    auto err = message.encode(encoder, out);
    TEST_ASSERT_EQUAL(EncodeError::None, err);
    for (size_t i = 0; i < out.size(); i++) {
        TEST_ASSERT_EQUAL(0, out[i]);
    }
};

void run_pedal_message_tests() {
    RUN_TEST(test_pedal_binary_encoding);
};