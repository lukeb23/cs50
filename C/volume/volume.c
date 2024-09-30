// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file

    uint8_t header[HEADER_SIZE];

    // Copy header file into output - inspiration from Brian @ CS50

    fread(header, sizeof(header), 1, input);

    fwrite(header, sizeof(header), 1, output);



    // TODO: Read samples from input file and write updated data to output file

    // Declare sample variable
    int16_t exchange;

    // Print out updated sound value of indivdual sample
    while (fread(&exchange, sizeof(exchange), 1, input))
    {
        exchange = exchange * factor;

        // Write new audio sample in output
        fwrite(&exchange, sizeof(exchange), 1, output);
    }



    // Close files
    fclose(input);
    fclose(output);
}
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
  // Check for correct number of arguments
    if (argc != 2)
    {
        printf("Usage: ./recover.c IMAGE");
        return 1;
    }

    FILE *camera = fopen(argv[1], "r");

    if (camera == NULL)
    {
      printf("Image cannot be opened");
      return 1;
    }

    typedef uint8_t BYTE;

    int jpegsize = 512;

    ypedef uint8_t BYTE;

    char *files = malloc(8);

    int checkImg = 0;




    while(fread(&buffer, sizeof(BYTE), 512, camera) == 512)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0)  == 0xe0)
        {

            sprintf(files, "%03i.jpg", checkImg);
            FILE *output = NULL;


            if (checkImg == 0)
            {
              output = fopen(files, "w");
              fwrite(&buffer, sizeof(BYTE), 512, output);
              checkImg++;
            }
            else
            {
              sprintf(files, "%03i.jpg", checkImg);
              output = fopen(files, "w");
              fwrite(&buffer, sizeof(BYTE), 512, output);
              checkImg++;
            }
            fclose(output);
        }

    }

fclose(camera);
return 0;


}