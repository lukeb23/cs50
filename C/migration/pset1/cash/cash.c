#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    // Prompt for amount of cash
    
    float cash;
    
    do
    {
        cash = get_float("Amount of cash: ");
    }
    
    while (cash <= 0);
    
    
    // Convert cash to cents
    
    int cents = round(cash * 100);
    
    
    // Deduce maximum amount of coins needed
    
    int change_amount = 0;

        
    while (cents >= 25)
    {
        cents = cents - 25;
        change_amount++;
    }
        
    while (cents >= 10 && cents < 25)
    {
        cents = cents - 10;
        change_amount++;
    }
        
    while (cents >= 5 && cents < 10)
    {
        cents = cents - 5;
        change_amount++;
    }
        
    while (cents >= 1 && cents < 5)
    {
        cents = cents - 1;
        change_amount++;
        
    }


    // Print amount of change
    
    printf("Change owed: %i\n", change_amount);
    
    
}