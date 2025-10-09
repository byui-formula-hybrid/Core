#pragma once

/**
 * @file Core.h
 * @brief Public interface for the Core library
 * 
 * This is the main header that consumers should include to access
 * all Core library functionality for reactive programming on ESP32.
 * 
 * Usage:
 *   #include <Core.h>
 *   using namespace Core;
 *   
 *   // For ESP32 with thread safety:
 *   auto lockStrategy = std::unique_ptr<LockStrategy>(new FreeRTOSLockStrategy());
 *   auto publisher = PublisherFactory::create<float>(std::move(lockStrategy));
 *   
 *   // For single-threaded or externally synchronized environments:
 *   auto publisher = PublisherFactory::createUnsafe<float>();
 *   
 *   auto subscription = publisher.sink([](float value) {
 *       Serial.println(value);
 *   });
 *   publisher.send(42.0f);
 * 
 * @version 1.0.0
 * @author Formula Hybrid Team
 */

// Core reactive programming components
#include "reactive/Publisher.h"
#include "reactive/CurrentValueSubject.h"
#include "reactive/Subscription.h"

// Factory classes for easy object creation
#include "factories/PublisherFactory.h"
#include "factories/CurrentValueSubjectFactory.h"

// Strategy interface and implementations
#include "strategies/LockStrategy.h"
#include "strategies/FreeRTOSLockStrategy.h"

// Note: Test mocks are not exposed in the public API