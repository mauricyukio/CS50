# Draws a pyramid with height defined by the user

from cs50 import get_int

# Prompts user for name while name is not between 1 and  8
while True:
    height = get_int("Choose pyramid height (from 1 to 8): ")
    if height >= 1 and height <= 8:
        break

# Prints the pyramid
for i in range(0, height):
    print(" " * (height - i - 1), end="")
    print("#" * (i + 1), end="")
    print()