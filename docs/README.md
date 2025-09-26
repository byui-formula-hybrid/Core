# Goals
- Core Library
  - Modular
  - Business Logic
  - Separate from Flashing/Embedded Systems
  - Testable
  - Dependency Management (Conan)
  - Building (CMake)

- Embedded Library
  - Modular
  - Separate from Core Business Logic
  - Flashing/Building and Dependency Management(PlatformIO)

# Unknowns/TODOs
- Structure of C++ Library (examples)
- Formatting of C++ Code (is there a dependency we can run at compile time or prior that can help enforce this)
- CMake (examples of how to setup and perform on any platform -- Linux, MacOS, Windows)
- PlatformIO (example of how to setup and perform on any platform -- Linux MacOS, Windows)
- Design Documents
  - Big Picture
  - Smaller Pictures (Zoom into from big picture)
    - VCM
    - Pedal
    - Display