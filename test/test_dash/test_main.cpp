#include <unity.h>
#include "test_main.h"

// Setup function called before each test
void setUp(void) {
    // No platform-specific setup needed for pure reactive components
}

// Teardown function called after each test
void tearDown(void) {
    // Clean up any test artifacts
}

int main() {
    UNITY_BEGIN();
    
    // Phase 1: CAN Message encode/decode tests
    RUN_TEST(test_ams_volt_current_encode_decode);
    RUN_TEST(test_ams_temperature_encode_decode);
    RUN_TEST(test_ams_fault_encode_decode);
    RUN_TEST(test_imd_fault_encode_decode);
    RUN_TEST(test_motor_status_encode_decode);
    RUN_TEST(test_vcu_status_encode_decode);
    RUN_TEST(test_pedal_data_encode_decode);
    RUN_TEST(test_pedal_data_with_error_encode_decode);
    RUN_TEST(test_heartbeat_encode_decode);
    RUN_TEST(test_heartbeat_counter_rollover);
    RUN_TEST(test_canid_constants);
    RUN_TEST(test_message_sizes);
    
    // Phase 2: Data Model & Controller tests
    // (to be added in Phase 2)
    
    // Phase 3: Pedal Error Code tests
    // (to be added in Phase 3)
    
    return UNITY_END();
}