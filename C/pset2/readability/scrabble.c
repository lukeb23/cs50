#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner

    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // TODO: Compute and return score for string


    int word_score = 0;


    for (int i = 0, len = strlen(word); i < len; i++)
    {
        word[i] = toupper(word[i]);
        word_score += POINTS[word[i] - 'A'];

    }

    return word_score;
}

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

bool letter_check(string text);
string encrypter(string key, string text);


int main(int argc, string argv[])
{
    string key = argv[1];

// Check for correct number of arguments
    if (argc != 2)
    {
        printf("How to use: ./substitution key\n");
        return 1;
    }

// Checks for correct submission of key
    else if (letter_check(key) == false)
    {
        printf("Must contain 26 letters\n");
        return 1;
    }

// Prompt for plaintext
    string plain_text = get_string("Plaintext: ");

// Print encrypted text
    printf("Ciphertext: %s\n", encrypter(key, plain_text));

    return 0;
}


// Function for the correct submission of key - 26 letters only

bool letter_check(string text)
{
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        for (int j = 26, z = len; j > i; j--)
        {
            text[i] = toupper(text[i]);
            text[j] = toupper(text[j]);
            if (!isalpha(text[i]) || text[i] == text[j] || strlen(text) != 26)
            {
                return false;
            }
        }
    }
    return true;
}


// Encrypt plaintext using the key

string encrypter(string key, string text)
{

    for (int i = 0, len = strlen(text); i < len; i++)
    {
        // Encrypt uppercase letters
        if (isupper(text[i]))
        {
            text[i] = toupper(key[text[i] - 'A']);

        }
        //Encrypt lowercase letter
        else
        {
            text[i] = tolower(key[text[i] - 'a']);
        }

    }
    return text;
}
