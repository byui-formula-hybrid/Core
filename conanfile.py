from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout, CMakeToolchain, CMakeDeps

class Core(ConanFile):
    name = "core"
    version = "1.0.0"
    license = "MIT"
    author = "BYU-Idaho Formula Hybrid"
    url = "https://github.com/byui-formula-hybrid/Core"
    description = "A modular C++ library for the BYU-Idaho Formula Hybrid team."

    # Binary configuration
    settings = "os", "arch", "compiler", "build_type"

    # Options that can be added at compile time.
    # Unsure if we want to utilize this but if we do logging might be useful.
    # To use this in the code, use #ifdef CORE_LOGGING
    options = {
      "logging": [True, False]
    }
    default_options = {
        "logging": False
    }

    # This is used to specify which files to include in the package.
    exports_sources = "CMakeLists.txt", "modules/*"
    
    def requirements(self):
        self.requires("gtest/1.14.0")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        CMakeToolchain(self).generate()
        CMakeDeps(self).generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        # Core module
        self.cpp_info.components["core"].includedirs = ["modules/core/include"]
        self.cpp_info.components["core"].srcdirs = ["modules/core/src"]
        self.cpp_info.components["core"].libs = ["corelib_core"]

        # Battery module
        self.cpp_info.components["battery"].includedirs = ["modules/battery/include"]
        self.cpp_info.components["battery"].srcdirs = ["modules/battery/src"]
        self.cpp_info.components["battery"].libs = ["corelib_battery"]
        # Example of future dependency:
        # self.cpp_info.components["battery"].requires = ["core"]

        # CAN module
        self.cpp_info.components["can"].includedirs = ["modules/can/include"]
        self.cpp_info.components["can"].srcdirs = ["modules/can/src"]
        self.cpp_info.components["can"].libs = ["corelib_can"]
        # self.cpp_info.components["can"].requires = ["core"]

        # Display module
        self.cpp_info.components["display"].includedirs = ["modules/display/include"]
        self.cpp_info.components["display"].srcdirs = ["modules/display/src"]
        self.cpp_info.components["display"].libs = ["corelib_display"]

        # IMD module
        self.cpp_info.components["imd"].includedirs = ["modules/imd/include"]
        self.cpp_info.components["imd"].srcdirs = ["modules/imd/src"]
        self.cpp_info.components["imd"].libs = ["corelib_imd"]

        # Inverter module
        self.cpp_info.components["inverter"].includedirs = ["modules/inverter/include"]
        self.cpp_info.components["inverter"].srcdirs = ["modules/inverter/src"]
        self.cpp_info.components["inverter"].libs = ["corelib_inverter"]

        # Pedals module
        self.cpp_info.components["pedals"].includedirs = ["modules/pedals/include"]
        self.cpp_info.components["pedals"].srcdirs = ["modules/pedals/src"]
        self.cpp_info.components["pedals"].libs = ["corelib_pedals"]

        # Pump module
        self.cpp_info.components["pump"].includedirs = ["modules/pump/include"]
        self.cpp_info.components["pump"].srcdirs = ["modules/pump/src"]
        self.cpp_info.components["pump"].libs = ["corelib_pump"]