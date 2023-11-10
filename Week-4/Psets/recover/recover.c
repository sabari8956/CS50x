#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // checks for data
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // opening raw file from memory card
    FILE *f = fopen(argv[1], "r");
    // if filename doesn't exist
    if (f == NULL)
    {
        printf("./recover card.raw\n");
    }
    // prototype fr image
    FILE *img = NULL;

    // filename
    char filename[8];
    // jpeg consists of 512 bytes
    int block_size = 512;
    int no_images = 0;
    BYTE data[block_size];

    while (fread(data, 1, block_size, f) == block_size)
    {
        // checks for staring of a jpeg file
        if (data[0] == 0xff && data[1] == 0xd8 && data[2] == 0xff && (data[3] & 0xf0) == 0xe0)
        {
            if (no_images == 0)
            {
                // first jpeg
                sprintf(filename, "%03i.jpg", no_images);
                img = fopen(filename, "w");
                fwrite(data, sizeof(BYTE), block_size, img);
                no_images++;
            }

            else
            {
                //closes the old image
                fclose(img);
                // opens the next new file
                sprintf(filename, "%03i.jpg", no_images);
                img = fopen(filename, "w");
                fwrite(data, sizeof(BYTE), block_size, img);
                no_images++;
            }
        }
        else if (no_images != 0)
        {
            fwrite(data, sizeof(BYTE), block_size, img);
        }
    }
    // closes all files
    fclose(img);
    fclose(f);
}