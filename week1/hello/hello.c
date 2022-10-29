// Says hello to the world

#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Gets user input
    string name = get_string("What's your name? ");
    // Prints message
    printf("Hello, %s\n", name);
}