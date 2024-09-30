#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

float letter_count(string text);
int word_count(string text);
float sentence_count(string text);
string book_text;


int main(void)
{
    
    // Prompt User for text

    book_text = get_string("Text: ");


    // Calculate the index

    float index = 0.0588 * letter_count(book_text) - 0.296 * sentence_count(book_text) - 15.8;


    //Print results

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }

    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }

    else
    {
        printf("Grade %.0f\n", index);
    }

}


//Function for getting word count

int word_count(string text)
{
    float words = 1;

    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (text[i] == 32)
        {
            words++;
        }
    }

    return words;
}


// Function to get letters per 100 words


// Get letter amount

float letter_count(string text)
{
    float letters = 0;

    for (int i = 0, len = strlen(text); i < len; i++)
    {
        text[i] = toupper(text[i]);

        if (text[i] >=  'A' && text[i] <= 'Z')
        {
            letters++;
        }

    }

// Get letter to word ratio

    float letter_ratio = letters / word_count(book_text) * 100;

    return letter_ratio;
}


// Function to get sentences per 100 words


// Get sentence amount

float sentence_count(string text)
{
    float sentences = 0;

    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            sentences++;
        }
    }

// Get word to sentence ratio

    float sentence_ratio = sentences / word_count(book_text) * 100;

    return sentence_ratio;
}