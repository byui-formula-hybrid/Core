#!/usr/bin/env python3
import subprocess
import sys
from shared import print_header, print_success, print_error, command_exists

def main():
    print_header("Install Script")

    # Check for Python
    if not command_exists('python3'):
        print_error("Python3 is required.")
        sys.exit(1)

    # Check for pip
    if not command_exists('pip3'):
        print_error("pip3 is required.")
        sys.exit(1)

    # Check for PlatformIO
    if not command_exists('pio'):
        print_success("PlatformIO already installed.")
    else:
        subprocess.run([sys.executable, '-m', 'pip', 'install', '--user', 'platformio'], check=True)
        print_success("PlatformIO installed.")

    # Install project dependencies
    subprocess.run(['pio', 'run', '-e', 'esp32dev'], check=True)
    print_success("Project dependencies installed.")

if __name__ == "__main__":
    main()