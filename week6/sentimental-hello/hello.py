# Says hello to the name provided

from cs50 import get_string

# Prompts user for name while name is not alphanumeric
while True:
    name = get_string("Name: ")
    if name.isalpha():
        break

print("Hello, " + name)