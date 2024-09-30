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

    if (argc == 1)
    {
        return 1;
    }
    else if (argc != 2)
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
    string plain_text = get_string("plaintext: ");

// Print encrypted text

    printf("ciphertext: %s\n", encrypter(key, plain_text));

    return 0;
}


// Function for the correct submission of key - 26 letters only

bool letter_check(string text)
{
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        for (int j = 26; j > i; j--)
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
        else if (islower(text[i]))
        {
            text[i] = tolower(key[text[i] - 'a']);
        }
        else if (text[i] == '\0')
        {
            text[i] = text[i];
        }

    }
    return text;
}