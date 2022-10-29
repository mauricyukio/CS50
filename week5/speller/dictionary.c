// Implements a dictionary's functionality

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <cs50.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

// Global variable to count the number of words in the dictionary
int g_counter = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hashes the word to obtain a hash value
    int index = hash(word);

    // Sets cursor pointer to run across the linked list
    node *cursor = table[index];

    // Until cursor reaches the end of the linked list, aka points to NULL
    while (cursor != NULL)
    {
        // Checks if the word is in the dictionary
        if (strcasecmp((cursor -> word), word) == 0)
        {
            return true;
        }
        // Moves the cursor one step forward
        cursor = cursor -> next;
    }
    return false;
}

// Hashes word to a number
// Reference: https://www.youtube.com/watch?v=jtMwp0FqEcg
unsigned int hash(const char *word)
{
    int total = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        total += i * toupper(word[i]);
    }
    return total % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Opens the dictionary file
    FILE *file = fopen(dictionary, "r");

    // Checks if there is available memory
    if (file == NULL)
    {
        return false;
    }

    // Initializes a buffer to store the word read from file
    char buffer[LENGTH + 1];

    // For each string in the dictionary, until the end of the file, copy the string to a buffer
    while (fscanf(file, "%s", buffer) != EOF)
    {
        // Dynamically allocates space in memory for a new node
        node *entry = malloc(sizeof(node));

        // Checks if there is available memory
        if (entry == NULL)
        {
            return false;
        }

        // Populates the node.word by copying the word from the buffer
        strcpy(entry -> word, buffer);

        // Gets the hash index for the word
        int index = hash(buffer);

        // Inserts new node into the linked list corresponding to the hash index
        // If it is the first node in the linked list associated to this bucket, point to NULL
        if (table[index] == NULL)
        {
            entry -> next = NULL;
        }
        // Otherwise, point to the existing first node in the linked list
        else
        {
            entry -> next = table[index];
        }
        // Then set the header pointer to point at the node. No nodes orphaned
        table[index] = entry;

        // Updates counter
        g_counter++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return g_counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        // If there are nodes after this header
        if (table[i] != NULL)
        {
            // Sets cursor pointer to run across the linked list
            node *cursor = table[i];

            // Sets temp pointer to point to the node to be freed from memory
            node *temp = table[i];

            // Until cursor reaches the end of the linked list, aka points to NULL
            do
            {
                // Moves the cursor one step forward
                cursor = cursor -> next;

                // Frees the node temp is pointing to
                free(temp);

                // Resets the temp pointer to point at the same node as the cursor
                temp = cursor;
            }
            while (cursor != NULL);
        }
    }
    return true;
}