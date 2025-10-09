#!/usr/bin/env python3
import argparse
import subprocess
import sys
from shared import print_header, print_error, print_success

def main():
    parser = argparse.ArgumentParser(description="Build project")
    parser.add_argument('--env', default='esp32dev', help='PlatformIO environment')
    parser.add_argument('--clean', action='store_true', help='Clean build files')
    parser.add_argument('--upload', action='store_true', help='Upload to device')
    parser.add_argument('--monitor', action='store_true', help='Open serial monitor')
    args = parser.parse_args()

    print_header("Build Script")

    if args.clean:
        subprocess.run(['pio', 'run', '-e', args.env, '-t', 'clean'], check=True)
        print_success("Cleaned build files.")

    subprocess.run(['pio', 'run', '-e', args.env], check=True)
    print_success(f"Built for environment: {args.env}")

    if args.upload:
        subprocess.run(['pio', 'run', '-e', args.env, '-t', 'upload'], check=True)
        print_success("Uploaded to device.")

    if args.monitor:
        subprocess.run(['pio', 'device', 'monitor', '-e', args.env], check=True)

if __name__ == "__main__":
    main()