#include <battery.h>
#include <can.h>
#include <core.h>
#include <dash.h>
#include <imd.h>
#include <inverter/DTIX50.h>
#include <pedals.h>
#include <pump.h>

// Minimal test for Inverter::DTIX50::Heartbeat using mocks
#include <memory>
#include "../test/mocks/mocks.h"
#include <inverter/DTIX50/heartbeat.h>
#include <can/provider.h>

int main(void) {
	// Create mock CAN service and provider
	auto mockService = std::make_shared<MOCKS::MockCanService>();
	auto canProvider = std::make_shared<CAN::Provider>(mockService.get());
	// Use native lock and thread strategies for host testing
	auto lockStrategy = std::unique_ptr<MOCKS::NativeLockStrategy>(new MOCKS::NativeLockStrategy());
	auto threadStrategy = std::unique_ptr<MOCKS::NativeThreadStrategy>(new MOCKS::NativeThreadStrategy());

	Inverter::DTIX50::Heartbeat heartbeat(canProvider, std::move(lockStrategy), std::move(threadStrategy));
	heartbeat.start();
	heartbeat.stop();
	return 0;
}