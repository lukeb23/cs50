#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    
    string card_number = get_string("Your card number: ");
    
    char *remaining;
    
    if (strlen(card_number) != 13 && strlen(card_number) != 15 && strlen(card_number) != 16)
    {
        printf("INVALID\n");
    }
    else
    {
        long numerical_card = strtol(card_number, &remaining, 10);
        
        numerical_card = numerical_card * 10 -
    }
    
}