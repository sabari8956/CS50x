#include <cs50.h>
#include <stdio.h>
#include <math.h>

typedef struct
{
    int l;
    int w;
    int s;
}
lws;

lws count(string text);

int main(void)
{
    string sentence = get_string("TEXT: ");
    lws data = count(sentence);
    // seprerates data from struct
    int letters = data.l;
    int words = data.w ;
    int sentences = data.s;

    // algorithm
    float L = (float) letters / (float) words * 100;
    float S = (float) sentences / (float) words * 100;

    int index = round(0.0588 * L - 0.296 * S - 15.8);
    // grading
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

lws count(string text)
{
    int n = 0;
    // defining variables
    int letters = 0;
    int words = 1;
    int sentences = 0;
    while (text[n] != '\0')
    {
        // letters
        int s_letter = text[n] - 'a';
        int b_letter = text[n] - 'A';
        if ((s_letter > - 1 && s_letter < 26) || (b_letter > - 1 && b_letter < 26))
        {
            letters++;
        }
        // words
        else if (text[n] == ' ')
        {
            words++;
        }

        // sentences
        else if (text[n] == '.' || text[n] == '!' || text[n] == '?')
        {
            sentences++;
        }
        n++;
    }
    // pushes to struct
    lws data = {letters, words, sentences};
    return data;
}
