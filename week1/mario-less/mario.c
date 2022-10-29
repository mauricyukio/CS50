// Draws a semi-pyramid made of # and with height inputted by the user

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;

    // Prompts user for a "height" value between 1 and 8
    do
    {
        height = get_int("Input pyramid's height (positive integer between 1 and 8): ");
    }
    while ((height < 1) || (height > 8));

    // Draws the pyramid
    for (int i = 0; i < height; i++)
    {
        for (int k = height - 1; k > i; k--)
        {
            printf(" ");
        }
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}