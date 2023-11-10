#include <cs50.h>
#include <stdio.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // check for key
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    string key = argv[1];
    // check for len of key
    if (key[26] != '\0')
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    // checks for duplicate in key
    for (int i = 0 ; i < 26 ; i++)
    {
        int val = 0;
        if (isupper(key[i]))
        {
            val = 65;
        }
        else if (islower(key[i]))
        {
            val = 97;
        }
        else
        {
            printf("Invalid key\n");
            return 1;
        }
        int alpha_i = key[i] - val;
        for (int j = 0 ; j < 26 ; j++)
        {
            int letter2 = key[j];
            if (i != j)
            {
                int val1 = 0;
                if (isupper(key[j]))
                {
                    val1 = 65;
                }
                else if (islower(key[j]))
                {
                    val1 = 97;
                }
                else
                {
                    printf("Invalid key\n");
                    return 1;
                }
                int alpha_j = key[j] - val1;
                if (alpha_i == alpha_j)
                {
                    printf("Invalid key\n");
                    return 1;
                }

            }

        }
    }

    int n  = 0 ;
    int base_c_val = 65;
    int base_s_val = 97;
    int keys[26];
    // calculate keys
    while (key[n] != '\0')
    {
        char letter = key[n];
        int key_temp;
        if (isupper(letter))
        {
            key_temp =  letter - base_c_val;
        }
        else
        {
            key_temp =  letter - base_s_val;
        }
        keys[n] = key_temp;
        // increses the alphabet
        base_c_val ++;
        base_s_val++;
        n++;
    }

    // get input
    string text = get_string("TEXT: ");
    n = 0;
    while (text[n] != '\0')
    {
        char letter = text[n];
        // ascii value
        int ascii_val = letter;
        // alphabet number/ value
        int alpha_val ;

        if (isupper(letter))
        {
            alpha_val = ascii_val - 'A';
            text[n] = keys[alpha_val] + ascii_val;
        }
        else if (islower(letter))
        {
            alpha_val = ascii_val - 'a';
            text[n] = keys[alpha_val] + ascii_val;
        }
        n++;
    }
    printf("ciphertext: %s\n", text);

    return 0;

}