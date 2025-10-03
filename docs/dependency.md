# Dependency Injection in the Race Car Software Project

## Overview

This document introduces **Dependency Injection (DI)** and explains how we can use a DI framework, specifically **[Boost.DI](https://boost-experimental.github.io/di/)**, in our race car software project. It is designed for new developers, students, and contributors who want to understand how our software is structured and how to maintain or extend it.

---

## What is Dependency Injection?

Dependency Injection is a **software design pattern** that allows you to separate the creation of an object from its dependencies. Instead of hardcoding dependencies inside your classes, DI lets you **inject them from the outside**, usually through constructors or setters.

### Why it matters

* **Improves modularity:** Each component only depends on abstractions (interfaces), not concrete implementations.
* **Simplifies testing:** You can easily swap real dependencies with mocks or stubs.
* **Encourages maintainability:** Changing one implementation doesn’t require changing dependent classes.
* **Promotes reusability:** Components can be used in multiple contexts without modification.

---

## How a DI Framework Helps

Manually wiring dependencies can become complex in large systems, like our race car software where we have multiple modules for pedals, display, network, sensors, and control logic.

A **Dependency Injection framework** like Boost.DI:

* **Automatically resolves dependencies:** The framework creates objects and injects required dependencies based on your configuration.
* **Manages object lifetimes:** You can define whether objects are **singletons**, **transient**, or **shared**, without writing custom factory code.
* **Reduces boilerplate code:** No need to manually pass dependencies through multiple constructors.
* **Encourages consistency:** All modules follow a clear, uniform method for obtaining dependencies.

Example in C++ using Boost.DI:

```cpp
#include <boost/di.hpp>
namespace di = boost::di;

struct Engine {
    void start() {}
};

struct Car {
    Car(Engine& e) : engine(e) {}
    Engine& engine;
};

int main() {
    auto injector = di::make_injector(
        di::bind<Engine>().to<Engine>().in(di::singleton)
    );

    auto car = injector.create<Car>();
    car.engine.start();
}
```

Here, Boost.DI automatically **creates the Engine instance** and **injects it into the Car constructor**.

---

## Multiple Bindings and Injectors

You can register multiple bindings when creating an injector. This allows you to define all your module interfaces and implementations in one place:

```cpp
auto injector = di::make_injector(
    di::bind<IThrottleController>.to<ThrottleController>().in(di::singleton),
    di::bind<IDisplay>.to<DisplayModule>().in(di::singleton),
    di::bind<ISensorManager>.to<SensorManager>().in(di::shared)
);
```

Each binding ensures that the correct implementation is injected wherever the interface is required.

### Using Multiple Injectors

* **One master injector** is generally recommended for a project like this. It centralizes all bindings and makes it easier to manage lifetimes, especially for shared and singleton components.
* If you need isolated configurations for testing or simulation, you can create **separate injectors** with only the bindings needed for that scenario. For example, you can swap a `MockSensorManager` in a test injector while keeping the master injector unchanged.

**Tip:** Avoid creating multiple injectors for normal application flow; prefer one central injector for consistency.

---

## Why We Recommend DI in Our Race Car Software

Our project has multiple subsystems:

* Pedal controller
* Display interface
* Sensor and telemetry processing
* Core control logic

Using DI provides:

1. **Clear separation of concerns** – each module only knows about the interfaces it needs.
2. **Simpler testing** – individual modules can be tested in isolation.
3. **Flexible replacement of components** – e.g., swapping a simulation engine for a real engine during development.

We suggest adopting Boost.DI across our software modules to **standardize how dependencies are created and shared**, making it easier for new contributors to understand and extend the project.

---

## Recommended Approach

1. **Define interfaces** for each module, e.g., `IPedalController`, `IDisplay`.
2. **Implement concrete classes** for these interfaces.
3. **Bind interfaces to implementations** in a central injector (e.g., `di::injector`).
4. **Inject dependencies** using constructors or parameters.
5. **Use singleton or shared scope** for components that should exist once, like a sensor manager or network module.

---

## Quick Start: Adding a New Module

This section walks you through adding a new module to our dependency injection system using Boost.DI.

### 1. Define an Interface

```cpp
// IThrottleController.h
struct IThrottleController {
    virtual void setThrottle(double value) = 0;
    virtual ~IThrottleController() = default;
};
```

### 2. Implement the Concrete Class

```cpp
// ThrottleController.h
#include "IThrottleController.h"
#include <iostream>

struct ThrottleController : IThrottleController {
    void setThrottle(double value) override {
        std::cout << "Throttle set to " << value << "%" << std::endl;
    }
};
```

### 3. Register the Module in the Injector

```cpp
#include <boost/di.hpp>
#include "IThrottleController.h"
#include "ThrottleController.h"

namespace di = boost::di;

auto injector = di::make_injector(
    di::bind<IThrottleController>.to<ThrottleController>().in(di::singleton)
);
```

### 4. Inject Dependencies

```cpp
struct Car {
    Car(IThrottleController& throttle) : throttleController(throttle) {}

    void accelerate(double value) {
        throttleController.setThrottle(value);
    }

private:
    IThrottleController& throttleController;
};

int main() {
    auto car = injector.create<Car>();
    car.accelerate(75); // Uses the injected ThrottleController
}
```

### 5. Testing Modules

```cpp
struct MockThrottleController : IThrottleController {
    void setThrottle(double value) override {
        // Do nothing or log for testing
    }
}

// Injector for tests
auto testInjector = di::make_injector(
    di::bind<IThrottleController>.to<MockThrottleController>()
);
```

✅ **Summary:**

1. Define interface
2. Implement concrete class
3. Bind in injector with proper scope
4. Inject into dependent classes
5. Test by swapping mocks

This workflow ensures **modular, testable, and maintainable code**, which is crucial for our race car software.

---

## Resources

* [Boost.DI Documentation](https://boost-experimental.github.io/di/)
* [Boost.DI GitHub Repository](https://github.com/boost-experimental/di)
* [Introduction to Dependency Injection (Wikipedia)](https://en.wikipedia.org/wiki/Dependency_injection)
* [C++ DI Example Projects](https://boost-experimental.github.io/di/examples.html)

---

This document is part of the **race car project documentation** and is intended to be referenced by future students and developers. By adopting DI, we aim to **simplify development, testing, and future expansion** of our software.

⚠️ **Note:** All new modules should follow the DI pattern and be registered in the central injector to ensure consistency across the project.
