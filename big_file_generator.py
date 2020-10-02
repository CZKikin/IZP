#!/usr/bin/python3
import sys

if (int(sys.argv[1]) > 0):
    with open("test_file", "w") as file:
        file.write(int(sys.argv[1]) * "A")
else:
    print("Pičo...\n");
