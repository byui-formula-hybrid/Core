#include <unity.h>

#include "../../lib/dash/ui/page_diagnostics.h"

using namespace Dash;

void test_pedal_error_to_text_none() {
    DataModel data;
    data.pedal_error_code = 0;
    data.pedal_sync_fault = false;

    TEST_ASSERT_EQUAL_STRING("", UI::pedal_error_to_text(data));
}

void test_pedal_error_to_text_code() {
    DataModel data;
    data.pedal_error_code = 7;
    data.pedal_sync_fault = false;

    TEST_ASSERT_EQUAL_STRING("Slew Rate Critical", UI::pedal_error_to_text(data));
}

void test_pedal_error_to_text_sync() {
    DataModel data;
    data.pedal_error_code = 0;
    data.pedal_sync_fault = true;

    TEST_ASSERT_EQUAL_STRING("Pedal Implausible", UI::pedal_error_to_text(data));
}

void run_dash_page_diagnostics_tests() {
    RUN_TEST(test_pedal_error_to_text_none);
    RUN_TEST(test_pedal_error_to_text_code);
    RUN_TEST(test_pedal_error_to_text_sync);
}
