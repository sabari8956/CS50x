#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0 ; j < width; j++)
        {
            int red, green, blue, sum, avg;
            float avg1;
            red = image[i][j].rgbtRed;
            green = image[i][j].rgbtGreen;
            blue = image[i][j].rgbtBlue;
            avg1 = (red + green + blue) / 3.0;
            avg = round(avg1);
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
        for (int i = 0; i < height; i++)
    {
        for (int j = 0 ; j < width; j++)
        {
            float red, green, blue;
            int sred, sgreen, sblue;
            red = image[i][j].rgbtRed;
            green = image[i][j].rgbtGreen;
            blue = image[i][j].rgbtBlue;

            sred = round(.393 * red + .769 * green + .189 * blue);
            sgreen = round(.349 * red + .686 * green + .168 * blue);
            sblue = round(.272 * red + .534 * green + .131 * blue);

            if(sred > 255)
            {
                sred = 255;
            }
            if (sgreen > 255)
            {
                sgreen = 255;
            }
            if (sblue > 255)
            {
                sblue = 255;
            }
            image[i][j].rgbtRed = sred;
            image[i][j].rgbtGreen = sgreen;
            image[i][j].rgbtBlue = sblue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height ; i++)
    {
        for (int j = 0; j < width / 2 ; j++)
        {
            RGBTRIPLE buffer ;
            int red, green, blue;
            buffer = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j -1 ] = buffer;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            buffer_image[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int RED = 0;
            int GREEN = 0;
            int BLUE = 0;
            float cnt = 0.0;

            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    int pointerx = i + x;
                    int pointery = j + y;

                    if (pointerx < 0 || pointery < 0 || pointerx >= height || pointery >= width)
                    {
                        continue;
                    }

                    RED += image[pointerx][pointery].rgbtRed;
                    GREEN += image[pointerx][pointery].rgbtGreen;
                    BLUE += image[pointerx][pointery].rgbtBlue;
                    cnt++;
                }
            }

            buffer_image[i][j].rgbtRed = round(RED / cnt);
            buffer_image[i][j].rgbtGreen = round(GREEN / cnt);
            buffer_image[i][j].rgbtBlue = round(BLUE / cnt);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            image[i][j] = buffer_image[i][j];
        }
    }
}
