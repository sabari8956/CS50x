#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // get key value
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    int x = 0;
    while (argv[1][x] != '\0')
    {
        int ascii_val = argv[1][x] ;
        if (ascii_val < 48 || ascii_val > 57)
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
        x++;
    }
    int key = atoi(argv[1]);

    // get string
    string text = get_string("TEXT: ");
    // loop over the string
    int n = 0 ;
    while (text[n] != '\0')
    {
        char letter = text[n];
        int base_val, alpha_val ;
        // separate small lettrs with caplital letters
        if (isupper(letter))
        {
            base_val = 65;
        }
        else if (islower(letter))
        {
            base_val = 97;
        }
        // separate small lettrs with caplital letters
        if (isalpha(letter))
        {
            // change the string
            alpha_val = letter - base_val ;
            int key1 = (alpha_val + key) % 26;
            base_val += key1 ;

            text[n] = base_val ;
        }

        n++;
    }
    printf("ciphertext: %s\n", text);
}