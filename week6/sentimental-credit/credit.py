# Determines whether a provided credit card number is valid according to Luhn’s algorithm

from cs50 import get_string

# Prompts user for the credit card number
while True:
    number = get_string("Number: ")
    if number.isnumeric() and len(number) in [13, 15, 16]:
        break
    else:
        print("INVALID")
        quit()

# Implements Luhn's algorithm to verify mathematical validity of the credit card number
total = 0

for i in range(2, len(number) + 1, 2):
    digit = number[len(number) - i]
    digit = int(digit)
    digit *= 2
    if digit >= 10:
        digit = digit // 10 + digit % 10
    total += digit

for i in range(1, len(number) + 1, 2):
    digit = number[len(number) - i]
    digit = int(digit)
    total += digit

# States whether the card is AMEX, MASTERCARD, VISA, or if the number is invalid
if total % 10 != 0:
    print("INVALID")
elif len(number) == 15 and number[0] == "3" and number[1] in ["4", "7"]:
    print("AMEX")
elif len(number) == 16 and number[0] == "5" and number[1] in ["1", "2", "3", "4", "5"]:
    print("MASTERCARD")
elif len(number) in [13, 16] and number[0] == "4":
    print("VISA")
else:
    print("INVALID")