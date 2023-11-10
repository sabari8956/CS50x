#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);

int main(void)
{
    // TODO

    // step 1
    // get str from user

    string str = get_string("MESSAGE: ");

    // step 2
    // iltriate over the string by each char

    int n = 0;
    while (str[n] != '\0')
    {
        //step 3
        // converting char to hex
        int hex_v = str[n];
        int vals[BITS_IN_BYTE];
        int x = 0;
        // step 4
        // convering hex to bin
        for (int i = 7; i >= 0 ; i--)
        {
            int decider = hex_v / pow(2, i);
            if (decider == 1)
            {
                hex_v -= pow(2, i);
            }
            // storing values in a array
            vals[x] = decider;
            x++;
        }
        // step 5
        //final step
        // goes over the loop and prints each bit
        for (int i = 0 ; i < 8; i++)
        {
            print_bulb(vals[i]);
        }
        // goes to next line
        printf("\n");
        n++;
    }

}

void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("⚫");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("🟡");
    }
}

