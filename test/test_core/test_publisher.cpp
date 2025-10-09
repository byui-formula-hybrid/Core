#include <unity.h>
#include <memory>

// Include test helpers and events
#include "../helpers/TestPublisherFactory.h"
#include "../mocks/events/TestEvent.h"

// Include the publisher components
#include "../../lib/core/reactive/Publisher.h"
#include "../../lib/core/reactive/CurrentValueSubject.h"

using namespace Core;

void test_publisher_creation() {
    Publisher<int> publisher;
    TEST_ASSERT_EQUAL(0, publisher.subscriberCount());
}

void test_publisher_with_custom_lock_strategy() {
    // Use factory to create thread-safe publisher for testing
    auto publisher = TestFactoryHelpers::createPublisher<int>();
    
    int received_value = 0;
    auto subscription = publisher.sink([&received_value](int value) {
        received_value = value;
    });
    
    publisher.send(42);
    TEST_ASSERT_EQUAL(42, received_value);
    
    // Test thread safety by ensuring the value was set correctly
    // (The NativeLockStrategy provides real mutex protection)
}

void test_publisher_sink_and_send() {
    Publisher<int> publisher;
    int received_value = 0;
    
    auto subscription = publisher.sink([&received_value](int value) {
        received_value = value;
    });
    
    TEST_ASSERT_EQUAL(1, publisher.subscriberCount());
    
    publisher.send(42);
    TEST_ASSERT_EQUAL(42, received_value);
}

void test_publisher_multiple_subscribers() {
    Publisher<int> publisher;
    int value1 = 0, value2 = 0;
    
    auto sub1 = publisher.sink([&value1](int value) { value1 = value; });
    auto sub2 = publisher.sink([&value2](int value) { value2 = value; });
    
    TEST_ASSERT_EQUAL(2, publisher.subscriberCount());
    
    publisher.send(100);
    TEST_ASSERT_EQUAL(100, value1);
    TEST_ASSERT_EQUAL(100, value2);
}

void test_publisher_automatic_unsubscribe() {
    Publisher<int> publisher;
    int received_value = 0;
    
    {
        auto subscription = publisher.sink([&received_value](int value) {
            received_value = value;
        });
        
        TEST_ASSERT_EQUAL(1, publisher.subscriberCount());
        publisher.send(50);
        TEST_ASSERT_EQUAL(50, received_value);
    } // subscription goes out of scope here
    
    TEST_ASSERT_EQUAL(0, publisher.subscriberCount());
    
    // No callback should be triggered
    received_value = 0;
    publisher.send(75);
    TEST_ASSERT_EQUAL(0, received_value);
}

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

void test_publisher_with_test_events() {
    Publisher<TestEvent> publisher;
    TestEvent received_event;
    bool event_received = false;
    
    auto subscription = publisher.sink([&](const TestEvent& event) {
        received_event = event;
        event_received = true;
    });
    
    TestEvent test_event(42, 123456);
    publisher.send(test_event);
    
    TEST_ASSERT_TRUE(event_received);
    TEST_ASSERT_EQUAL(42, received_event.value);
    TEST_ASSERT_EQUAL(123456, received_event.timestamp);
}

void run_publisher_tests() {
    RUN_TEST(test_publisher_creation);
    RUN_TEST(test_publisher_with_custom_lock_strategy);
    RUN_TEST(test_publisher_sink_and_send);
    RUN_TEST(test_publisher_multiple_subscribers);
    RUN_TEST(test_publisher_automatic_unsubscribe);
    RUN_TEST(test_current_value_subject);
    RUN_TEST(test_current_value_subject_no_initial_value);
    RUN_TEST(test_publisher_with_test_events);
}