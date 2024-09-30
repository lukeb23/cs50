// Implements a dictionary's functionality

#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "dictionary.h"
#include <string.h>
#include <strings.h>
#include <cs50.h>


int current_size = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 234;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO

    // Set cursor to bucket location
    node *cursor = table[hash(word)];

    // Run while cursor still has words to look through
    while (cursor != NULL)
    {
        // Check if the word is in the dicitonary and continue otherwise
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;

    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function

    // Get length of word

    int word_len = strlen(word);

    // Get first letter of word
    char first_letter = toupper(word[0]);

    // Declare a number to return hash value to
    int hash_loc = 0;

    // Hash words by word length, then by aphabetical order
    if (word_len == 3 || word_len == 1 ||  word_len == 2)
    {
        return hash_loc = first_letter - 'A';
    }

    if (word_len == 4 || word_len == 5)
    {
        return hash_loc = ((first_letter - 'A') + 26);
    }

    if (word_len == 6)
    {
        return hash_loc = ((first_letter - 'A') + 52);
    }

    if (word_len == 7)
    {
        return hash_loc = ((first_letter - 'A') + 78);
    }

    if (word_len == 8)
    {
        return hash_loc = ((first_letter - 'A') + 104);
    }

    if (word_len == 9)
    {
        return hash_loc = ((first_letter - 'A') + 130);
    }

    if (word_len == 10)
    {
        return hash_loc = ((first_letter - 'A') + 156);
    }

    if (word_len == 11 || word_len == 12)
    {
        return hash_loc = ((first_letter - 'A') + 182);

    }

    if (word_len > 12)
    {
        return hash_loc = (first_letter - 'A') + 207;
    }

    return hash_loc;

}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO

    // Declare a file to write dictionary into

    FILE *dictionary_words = fopen(dictionary, "r");

    if (dictionary_words == NULL)
    {
        return false;
    }

    // Declare the length of words

    char word[LENGTH + 1];

    // While loop to run through each word in dictionary

    while (fscanf(dictionary_words, "%s", word) != EOF)
    {
        node *w = malloc(sizeof(node));

        // Checking if word was read correctly
        if (w == NULL)
        {
            return false;
        }

        // Copy word to node and set its next pointer to null
        strcpy(w->word, word);

        w->next = NULL;

        // Hashing word into table
        int index = hash(w->word);

        // Point word to table index's next word, then table index to word
        w->next = table[hash(w->word)];

        table[index] = w;

        // update dictionary size
        current_size++;

    }

    fclose(dictionary_words);
    size();
    return true;

}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return current_size;

}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO

    // Initialize cursor, temp. cursor and freed word counter
    node *cursor = NULL;
    node *tmp = NULL;
    int freed = 0;

    // Run for the length of the table
    for (int i = 0; i < N; i++)
    {
        // set the cursor equal to the bucket location
        cursor = table[i];

        // Free the bucket with temp. after cursor has pointed to next word
        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
            freed++;
        }

    }

    // confirm amount of words freed is the amount of words in the dictionary
    if (freed == current_size)
    {
        return true;
    }

    return false;
}

