#pragma once

/**
 * @file Core.h
 * @brief Public interface for the Core library
 * 
 * This is the main header that consumers should include to access
 * all Core library functionality for reactive programming on ESP32.
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