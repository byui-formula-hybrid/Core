#!/usr/bin/env python3
import argparse
import subprocess
from shared import print_header, print_success

def main():
    parser = argparse.ArgumentParser(description="Run unit tests")
    parser.add_argument('--filter', help='Test filter (suite name)')
    parser.add_argument('--verbose', action='store_true', help='Verbose output')
    args = parser.parse_args()

    print_header("Test Suite")

    cmd = ['pio', 'test', '-e', 'native']
    if args.filter:
        cmd += ['--filter', args.filter]
    if args.verbose:
        cmd.append('-v')

    subprocess.run(cmd, check=True)
    print_success("Tests completed.")

if __name__ == "__main__":
    main()