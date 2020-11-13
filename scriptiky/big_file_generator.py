#!/usr/bin/python3
import sys
import random

if sys.argv[1] == "--help" or sys.argv[1] == "-h":
    print("""Usage: script pocet_radku <soubor_se_slovama
    """)
    quit()

w = sys.stdin.read() 
w = w.split()

if len(sys.argv) < 2:
    print("Missing argument")
    quit(-1)

with open("../test_file", "w") as file:
    for i in range(0, int(sys.argv[1])):
        for f in range(random.randint(0,60)):
            file.write(random.randint(0, 3) * str(random.choice(w)) + ":")
        file.write("\n")
