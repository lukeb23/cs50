# TODO
from cs50 import get_int

# Prompt for height until it fits parameters
while True:
    height = get_int("Height: ")
    if height > 0 and height < 9:
        break

copy = height - 1

# iterate over and overcreating pyramid

for i in range(height):
    for copy in range(copy):
        print(" ", end="")
    for k in range(i + 1):
        print("#", end="")
    print("  ", end="")
    for k in range(i + 1):
        print("#", end="")
    print()

