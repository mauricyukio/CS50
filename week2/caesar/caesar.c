// Encrypts a message using Caesar's cipher

#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool only_digits(string s);
char rotate(char c, int i);

int main(int argc, string argv[])
{
    // Checks if the user has passed exactly two arguments on the command line, the second one being the key
    // Also checks if every single character in the key provided is a number
    if (argc != 2 || !only_digits(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Converts the key provided from a string to an int
    int key = atoi(argv[1]);

    // Checks if the key provided is a positive integer. If not, exits the program
    if (key < 0)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Prompts the user for a plaintext to be encrypted
    string text = get_string("plaintext: ");

    // Encrypts the plaintext provided by rotating each character by as many positions as provided in the key
    int length = strlen(text);
    
    for (int i = 0; i < length; i++)
    {
        text[i] = rotate(text[i], key);
    }

    // Exhibits the final encrypted ciphertext
    printf("ciphertext: %s\n", text);

    return 0;
}

// Checks if every single character in a string is a number
bool only_digits(string s)
{
    int length = strlen(s);
    for (int i = 0; i < length; i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

// Rotates a character "c" by "i" positions in the alphabet
char rotate(char c, int i)
{
    if (!isalpha(c))
    {
        return c;
    }

    i %= 26;
    if (isupper(c))
    {
        c -= 65;
        c += i;
        if (c > 25)
        {
            return c + 65 - 26;
        }
        return c + 65;
    }

    c -= 97;
    c += i;
    if (c > 25)
    {
        return c + 97 - 26;
    }
    return c + 97;
}