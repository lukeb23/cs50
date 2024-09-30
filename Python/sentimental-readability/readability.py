# TODO
from cs50 import get_string, get_int

# Prompt User for text
book_text = get_string("Text ")


def main():

    # Calculate the index
    index = float(0.0588 * letter_count(book_text) - 0.296 * sentence_count(book_text) - 15.8)

    # Print results
    if index < 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {index:.0f}")


# Function for getting word count
def word_count(text):

    words = 1

    for i in range(len(text)):
        if ord(text[i]) == 32:
            words += 1
    return words


# Function to get letters per 100 words
def letter_count(text):

    letters = 0

    # Get letter count
    for i in range(len(text)):

        character = ord(text[i])

        character = ord(chr(character).upper())

        if character >= ord("A") and character <= ord("Z"):
            letters += 1

    # Get word to letter ratio
    letter_ratio = letters / word_count(book_text) * 100

    return letter_ratio


# Function to get sentences per 100 words
def sentence_count(text):

    # Get sentence amount
    sentences = 0

    for i in range(len(text)):
        if text[i] == "." or text[i] == "?" or text[i] == "!":
            sentences += 1

    # Get word to sentence ratio
    sentence_ratio = sentences / word_count(book_text) * 100

    return sentence_ratio


main()
