// Encrypts a message using a substitution cipher, with the key and the plaintext provided by the user

#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool only_letters(string s);
bool length_26(string s);
bool all_different(string s);
char substitute(char c, string ku, string kl);

int main(int argc, string argv[])
{
    // Validate Key
    // Checks if the user has passed exactly two arguments on the command line, the second one being the key
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Checks if every single character in the key provided is alphabetical
    if (!only_letters(argv[1]))
    {
        printf("Key must only contain alphabetic characters\n");
        return 1;
    }

    // Checks if the length of the key provided is exactly 26
    if (!length_26(argv[1]))
    {
        printf("Key must contain 26 characters\n");
        return 1;
    }

    // Checks if all 26 letters of the key are different from each other
    if (!all_different(argv[1]))
    {
        printf("Key must not contain repeated characters\n");
        return 1;
    }

    // Get Plaintext
    // Prompts the user for a plaintext to be encrypted
    string text = get_string("plaintext: ");

    // Encipher
    //Creates two additional strings, one being the key all uppercase and other being the key all lowercase
    string key_upper[26];
    string key_lower[26];

    int length_key = strlen(argv[1]);

    for (int i = 0; i < length_key; i++)
        {
            key_upper[i] = (argv[1][i]);
            key_lower[i] = (argv[1][i]);
            key_upper[i] = toupper(key_upper[i]);
            key_lower[i] = toupper(key_lower[i]);
        }

    // Encrypts the plaintext provided by substituting each character by the corresponding character from the key provided
    int length_text = strlen(text);

    for (int i = 0; i < length_text; i++)
    {
        text[i] = substitute(text[i], key_upper, key_lower);
    }

    // Print Ciphertext
    // Exhibits the final encrypted ciphertext
    printf("ciphertext: %s\n", text);

    return 0;
}

// Checks if every single character in a string is a letter
bool only_letters(string s)
{
    int length = strlen(s);

    for (int i = 0; i < length; i++)
    {
        if (!isalpha(s[i]))
        {
            return false;
        }
    }

    return true;
}

// Checks if the string's length is exactly 26
bool length_26(string s)
{
    int length = strlen(s);

    if (length != 26)
    {
        return false;
    }

    return true;
}

// Checks if all letters of a string are different from each other
bool all_different(string s)
{
    int length = strlen(s);

    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < length; j++)
        {
            if (i != j && s[i] == s[j])
            {
                return false;
            }
        }
    }

    return true;
}

// Substitutes a char for a corresponding char in a key. Case sensitive
char substitute(char c, string ku, string kl)
{
    int length = strlen(ku);

    if (!isalpha(c))
    {
        return c;
    }

    if (isupper(c))
    {
        return ku[c - 65];
    }
    else
    {
        return kl[c - 97];
    }
}