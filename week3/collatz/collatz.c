// Calculates how many steps it takes for a number to get back to 1 using the Collatz conjecture

#include <stdio.h>
#include <cs50.h>

int collatz(int n);

// int steps = 0;

int main(void)
{
    int n = get_int("Input your number: ");
    printf("It takes %i steps to get back to the number 1.\n", collatz(n));
}

int collatz(int n)
{
    // Base Case
    if (n == 1)
    {
        return 0;
    }

    // Recursive Cases
    if (n % 2 == 0)
    {
        // steps++;
        return 1 + collatz(n/2);
    }
    else
    {
        // steps++;
        return 1 + collatz(3 * n + 1);
    }
}