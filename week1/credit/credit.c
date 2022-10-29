// Checks whether the credit card number provided by the user is valid or not, and, if valid, whether it is from AmEx, MasterCard or VISA

#include <cs50.h>
#include <stdio.h>

long get_number(void);
string algorithm(long number);

int main(void)
{
    long number = get_number();
    string result = algorithm(number);
    printf("%s", result);
}

// Prompts user for input
long get_number(void)
{
    long number;
    do
    {
        number = get_long("Please insert a credit card number (no hyphens): ");
    }
    while (number < 0);
    return number;
}

// Checks if the number provided can be validated mathematically, and to which company it is associated
string algorithm(long number)
{
    int accumulator = 0;
    long position;
    long digit;

    // Starting from the decimal position, aka the second-to-last digit in the credit card number
    position = 10;
    while (number / position >= 1)
    {
        // Gets the digit associated with the current position
        digit = (number % (position * 10)) / position;
        digit *= 2;
        // Transforms a 2-digit number into the sum of its 2 digits
        if (digit >= 10)
        {
            digit = (digit / 10) + (digit % 10);
        }
        accumulator += digit;
        // Goes 2 digits to the left
        position *= 100;
    }

    // Starting again from the unity position, aka the last digit in the credit card number
    position = 1;
    while (number / position >= 1)
    {
        // Gets the digit associated with the current position
        digit = (number % (position * 10)) / position;
        accumulator += digit;
        // Goes 2 digits to the left
        position *= 100;
    }

    if (((accumulator % 10) != 0) || (number > 5599999999999999) || (number < 4000000000000))
    {
        return "INVALID\n";
    }
    else
    {
        // In case it is a mathematically valid credit card number
        if ((number / (position / 100) == 4) || (number / (position / 10) == 4))
        {
            return "VISA\n";
        }
        else if ((number / (position / 1000) == 34) || (number / (position / 100) == 34) ||
                 (number / (position / 1000) == 37) || (number / (position / 100) == 37))
        {
            return "AMEX\n";
        }
        else if ((number / (position / 1000) == 51) || (number / (position / 100) == 51) ||
                 (number / (position / 1000) == 52) || (number / (position / 100) == 52) ||
                 (number / (position / 1000) == 53) || (number / (position / 100) == 53) ||
                 (number / (position / 1000) == 54) || (number / (position / 100) == 54) ||
                 (number / (position / 1000) == 55) || (number / (position / 100) == 55))
        {
            return "MASTERCARD\n";
        }
        else
        {
            return "INVALID\n";
        }
    }
}