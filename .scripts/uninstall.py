#!/usr/bin/env python3
import os
import shutil
import sys
from shared import print_header, print_success, print_error, confirm

def remove_path(path):
    if os.path.isdir(path):
        shutil.rmtree(path)
    elif os.path.isfile(path):
        os.remove(path)

def main():
    print_header("Uninstall Script")
    if not confirm("Are you sure you want to uninstall and remove build artifacts?"):
        print_info("Uninstall cancelled.")
        sys.exit(0)

    # Remove build artifacts and virtual environments
    for path in ['.pio', '.venv', '__pycache__', 'build', 'dist']:
        remove_path(path)
    print_success("Removed build artifacts and environments.")

    # Uninstall PlatformIO
    subprocess.run([sys.executable, '-m', 'pip', 'uninstall', '-y', 'platformio'], check=True)
    print_success("PlatformIO uninstalled.")

if __name__ == "__main__":
    main()