#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Prompts user for text to be analysed
    string text = get_string("Text: ");

    // Counts the number of alphabetical characters in the provided text
    int letters = count_letters(text);

    // Counts the number of words in the provided text
    int words = count_words(text);

    // Counts the number of sentences in the provided text
    int sentences = count_sentences(text);

    // Calculates the Coleman-Liau index
    float L = (float) letters / words * 100;
    float S = (float) sentences / words * 100;

    int index = round(0.0588 * L - 0.296 * S - 15.8);

    // Prints out the grade that corresponds to the Coleman-Liau index
    if (index <= 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

// Counts the number of alphabetical characters in a string
int count_letters(string text)
{
    int letters = 0;
    int length = strlen(text);
    for (int i = 0; i < length; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
    }
    return letters;
}

// Counts the number of words in a string
int count_words(string text)
{
    int spaces = 0;
    int length = strlen(text);
    for (int i = 0; i < length; i++)
    {
        if (text[i] == 32)
        {
            spaces++;
        }
    }
    return (spaces + 1);
}

// Counts the number of sentences in a string
int count_sentences(string text)
{
    int sentences = 0;
    int length = strlen(text);
    for (int i = 0; i < length; i++)
    {
        if (text[i] == 33 || text[i] == 46 || text[i] == 63)
        {
            sentences++;
        }
    }
    return sentences;
}