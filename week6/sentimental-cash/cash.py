# Calculates the minimum number of coins required to give a user change

from cs50 import get_float

# Prompts user for the change owed (positive float)
while True:
    change = get_float("Change owed: ")
    if change >= 0:
        break

# Converts change from dollars to cents
change *= 100
change = int(change)

# Calculate coins amounts
quarters = change // 25
change = change - quarters * 25

dimes = change // 10
change = change - dimes * 10

nickels = change // 5
change = change - nickels * 5

pennies = change

print(quarters + dimes + nickels + pennies)