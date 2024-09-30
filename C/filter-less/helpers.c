#include "helpers.h"
#include <math.h>
#include <ctype.h>

// Convert image to grayscale

void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Declare variable to get the average
    float gsAverage = 0;

    // iterate over each row and column
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Determine the average rgb value
            gsAverage = round((image[i][j].rgbtGreen + image[i][j].rgbtBlue + image[i][j].rgbtRed) / 3.0);

            // Set the images rgb values to the average
            image[i][j].rgbtGreen = gsAverage;
            image[i][j].rgbtBlue = gsAverage;
            image[i][j].rgbtRed = gsAverage;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Declare variables to be filled
    int sRed = 0;
    int sGreen = 0;
    int sBlue = 0;

    // iterate over each row and column
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Use formula for sepia and check for numbers more than 255

            sRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            if (sRed > 255)
            {
                sRed = 255;
            }
            sGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            if (sGreen > 255)
            {
                sGreen = 255;
            }
            sBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);
            if (sBlue > 255)
            {
                sBlue = 255;
            }

            // Set rgb values to sepia values for each color
            image[i][j].rgbtGreen = sGreen;
            image[i][j].rgbtBlue = sBlue;
            image[i][j].rgbtRed = sRed;

        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    // Get half the width

    int half_width = width / 2;

    float half = width / 2;

    // Check if half width is rounded down for odd count

    if (half_width > half)
    {
        half_width--;
    }

    // Swap the pixel with its opposite counterpart using temporary variable
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < half_width; j++)
        {
            RGBTRIPLE temp = image[i][j];

            image[i][j] =  image[i][width - (j + 1)];

            image[i][width - (j + 1)] = temp;

        }

    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    // Declare and copy image onto a different image for reference values

    RGBTRIPLE imgcpy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imgcpy[i][j] = image[i][j];

        }
    }

    // Iterate over each column

    for (int i = 0; i < height; i++)
    {
        // Iterate over each row

        for (int j = 0; j < width; j++)
        {
            float blurRed = 0;
            float blurBlue = 0;
            float blurGreen = 0;
            int division = 0;

            // Iterate over each 3x3 group in a given area

            for (int z = i - 1; z <= i + 1; z++)
            {
                for (int y = j - 1; y <= j + 1; y++)
                {
                    // Check if the surrounding pixel exists in the image
                    if (z >= 0 && z < height && y >= 0 && y < width)
                    {
                        blurRed += imgcpy[z][y].rgbtRed;
                        blurGreen += imgcpy[z][y].rgbtGreen;
                        blurBlue += imgcpy[z][y].rgbtBlue;

                        division++;
                    }
                }
            }

            //Update pixel
            image[i][j].rgbtRed = round(blurRed / division);
            image[i][j].rgbtGreen = round(blurGreen / division);
            image[i][j].rgbtBlue = round(blurBlue / division);

        }
    }
    return;
}
