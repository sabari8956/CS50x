#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //gets height
    int height;
    do
    {
        height = get_int("enter Height: ");
    }

    //validates height
    while (1 > height || 9 <= height);

    // for each row
    for (int hases = 1; hases < height + 1; hases++)
    {
        int spaces, j = 0, z = 0;

        // calculates spaces
        spaces = height - hases;

        // prints spaces
        while (j < spaces)
        {
            printf(" ");
            j++;
        }

        // prints hases
        while (z < hases)
        {
            printf("#");
            z++;
        }

        // prints gap
        printf("  ");

        // prints oppsite hases
        z = 0;
        while (z < hases)
        {
            printf("#");
            z++;
        }

        // goes to next line
        printf("\n");
    }
}
