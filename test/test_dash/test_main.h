#pragma once

/**
 * @file test/test_dash/test_main.h
 * Dashboard module test entrypoint declarations.
 * 
 * Declares all test functions for each phase so they can be
 * registered with the Unity test framework.
 */

// Phase 1: CAN Message Tests
void test_ams_volt_current_encode_decode();
void test_ams_temperature_encode_decode();
void test_ams_fault_encode_decode();
void test_imd_fault_encode_decode();
void test_motor_status_encode_decode();
void test_vcu_status_encode_decode();
void test_pedal_data_encode_decode();
void test_pedal_data_with_error_encode_decode();
void test_heartbeat_encode_decode();
void test_heartbeat_counter_rollover();
void test_canid_constants();
void test_message_sizes();

// Phase 2: Data Model & Controller Tests
// (to be added in Phase 2)
// void test_controller_data_model_initialization();
// void test_controller_voltage_current_scaling();
// etc.

// Phase 3: Pedal Error Code Tests
// (to be added in Phase 3)
// void test_pedal_error_decode_no_error();
// void test_pedal_error_decode_fault();
// etc.
