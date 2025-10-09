import os
import sys
import platform
import shutil

def print_header(title):
    print(f"\n{'='*40}\n{title}\n{'='*40}")

def print_success(msg):
    print(f"\033[92m[SUCCESS]\033[0m {msg}")

def print_error(msg):
    print(f"\033[91m[ERROR]\033[0m {msg}")

def print_info(msg):
    print(f"\033[94m[INFO]\033[0m {msg}")

def is_windows():
    return platform.system() == "Windows"

def is_mac():
    return platform.system() == "Darwin"

def is_linux():
    return platform.system() == "Linux"

def command_exists(cmd):
    return shutil.which(cmd) is not None

def confirm(prompt):
    try:
        return input(f"{prompt} [y/N]: ").lower() == "y"
    except KeyboardInterrupt:
        print()
        return False