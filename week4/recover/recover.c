#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    // Keep track of how many JPG headers were found
    int counter = 0;

    // Pointer to the array in which to store the names of the files XXX.jpg\0
    char *filename = malloc(8 * sizeof(char));

    // Array in which to store the 512 bytes read from the input file
    BYTE buffer[BLOCK_SIZE];

    // Open input file for reading
    FILE *f = fopen(argv[1], "r");

    // Pointer to output file, to be assigned later
    FILE *img = NULL;

    // Check for appropriate usage of command line argument
    if (argc != 2)
    {
        printf("Correct usage: ./recover FILE\n");
        return 1;
    }

    // Check for readability of input file
    if (f == NULL)
    {
        printf("Could not open forensic image provided\n");
        return 1;
    }

    // Loop while there is still 512 bytes of information on the input file to read
    while (fread(buffer, BLOCK_SIZE, 1, f))
    {
        // Look for the start of a JPG file
        if (buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF && (buffer[3] & 0xF0) == 0xE0)
        {
            // Check whether it is the first JPG or not
            if (counter != 0)
            {
                // If it is not the first, close current JPG
                fclose(img);
            }

            // Generate next JPG file name, open it and start writting on it from the buffer
            sprintf(filename, "%03i.jpg", counter);
            img = fopen(filename, "w");
            fwrite(buffer, BLOCK_SIZE, 1, img);

            // Increase counter of successfully found JPG headers
            counter++;
        }

        // Write further on the current JPG while no other header is found
        else if (counter > 0)
        {
            fwrite(buffer, BLOCK_SIZE, 1, img);
        }
    }

    // Close currently open files and free dynamic allocated memory
    fclose(img);
    fclose(f);
    free(filename);

    return 0;
}