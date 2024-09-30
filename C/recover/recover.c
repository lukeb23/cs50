#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const int JPEGSIZE = 512;

int main(int argc, char *argv[])
{
    // Check to see if the user passes the right amount of arguments
    if (argc != 2)
    {
        printf("Usage: ./recover.c IMAGE");
        return 1;
    }

    // Open 

    FILE *camera = fopen(argv[1], "r");

    // Ensure that the camera has information
    if (camera == NULL)
    {
        printf("Image cannot be opened");
        return 1;
    }


    // Declare all the data used to translate the camera file
    typedef uint8_t BYTE;

    BYTE buffer[JPEGSIZE];

    char files[8];

    int checkImg = 0;

    FILE *output = NULL;


    // Read through all the camera data
    while (fread(&buffer, sizeof(BYTE), JPEGSIZE, camera))
    {
        // Check if the byte read is the first byte of a new photo
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0))
        {

            // Start writing if it is the first image
            if (checkImg == 0)
            {
                sprintf(files, "%03i.jpg", checkImg);
                output = fopen(files, "w");
                fwrite(&buffer, sizeof(BYTE), JPEGSIZE, output);
                checkImg++;

            }

            // Close the previous image before writing if not the first image
            else
            {
                fclose(output);
                sprintf(files, "%03i.jpg", checkImg);
                output = fopen(files, "w");
                fwrite(&buffer, sizeof(BYTE), JPEGSIZE, output);
                checkImg++;
            }

        }

        // Continue writing if first image has been found
        else if (checkImg > 0)
        {
            fwrite(&buffer, sizeof(BYTE), JPEGSIZE, output);
        }
    }
    // Close both files

    fclose(output);
    fclose(camera);

    return 0;

}