#include "unity.h"
#include "../../lib/pedals/pedal_message.h"
#include "../../lib/core/serialization/binary/encode/binary_encoder.h"

using namespace Pedals;
using namespace Core;

// TODO: What does the encoded object look like? Is it some kind of data object?
void test_pedal_binary_encoding() {
    PedalMessage message;
    BinaryEncoder encoder;
    auto err = message.encode(encoder);
    TEST_ASSERT_EQUAL(EncodeError::None, err);
};

void run_pedal_message_tests() {
    RUN_TEST(test_pedal_binary_encoding);
};