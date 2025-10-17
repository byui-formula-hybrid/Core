#include <unity.h>

#include "../helpers/test_publisher_factory.h"
#include "../../lib/core/observer/current_value_subject.h"

using namespace Core;

void test_current_value_subject() {
    auto subject = TestFactoryHelpers::createSubject<int>(25);
    
    TEST_ASSERT_TRUE(subject.hasValue());
    TEST_ASSERT_EQUAL(25, subject.value());
    
    int received_value = 0;
    auto subscription = subject.sink([&received_value](int value) {
        received_value = value;
    });
    
    // Should immediately receive current value
    TEST_ASSERT_EQUAL(25, received_value);
    
    // Send new value
    subject.send(30);
    TEST_ASSERT_EQUAL(30, received_value);
    TEST_ASSERT_EQUAL(30, subject.value());
}

void test_current_value_subject_no_initial_value() {
    auto subject = TestFactoryHelpers::createSubject<int>();
    
    TEST_ASSERT_FALSE(subject.hasValue());
    
    int received_value = -1;
    auto subscription = subject.sink([&received_value](int value) {
        received_value = value;
    });
    
    // Should not receive any value initially
    TEST_ASSERT_EQUAL(-1, received_value);
    
    // Send first value
    subject.send(100);
    TEST_ASSERT_EQUAL(100, received_value);
    TEST_ASSERT_TRUE(subject.hasValue());
    TEST_ASSERT_EQUAL(100, subject.value());
}

void run_value_subject_tests() {
    RUN_TEST(test_current_value_subject);
    RUN_TEST(test_current_value_subject_no_initial_value);
}