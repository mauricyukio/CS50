#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Gets input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Scores both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Prints the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
        printf("%i\n", score1);
        printf("%i\n", score2);
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
        printf("%i\n", score1);
        printf("%i\n", score2);
    }
    else
    {
        printf("Tie!\n");
        printf("%i\n", score1);
        printf("%i\n", score2);
    }
}

// Computes and returns score for string
int compute_score(string word)
{
    int length = strlen(word);
    int accumulator = 0;

    for (int i = 0; i < length; i++)
    {
        word[i] = tolower(word[i]);
        if (word[i] >= 97 && word[i] <= 122)
        {
            accumulator += POINTS[word[i] - 97];
        }
    }

    return accumulator;
}