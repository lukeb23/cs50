#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Get the height of the Pyramid (minimum 1, maximum 8)
    
    int height;
   
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);
   
   
    // Create Pyramid
     
    for (int i = 0; i < height; i++)
    {
        // Determine pyramid's indentation
        
        for (int z = height - 1; z > i && height > 1; z--)
        {
            printf(" ");
        }
        
        // Determine pyramid's length
        
        for (int j = -1; j < i; j++)
        {
            printf("#");
        }
        printf("  ");
        
        for (int y = -1; y < i; y++)
        {
            printf("#");
        }
 
        printf("\n");
    }
    
}