#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int starting_llamas;
    do
    {
        starting_llamas = get_int("Enter the Current llamas :");
    }
    while (starting_llamas < 9);
    // TODO: Prompt for end size
    int ending_llamas;
    do
    {
        ending_llamas = get_int("Enter the desired llamas :");
    }
    while (ending_llamas < starting_llamas);
    // TODO: Calculate number of years until we reach threshold
    int born_llamas, died_llamas, yrs = 0 ;
    while (starting_llamas < ending_llamas)
    {
        born_llamas = starting_llamas / 3;
        died_llamas = starting_llamas / 4;
        starting_llamas += born_llamas - died_llamas;
        yrs++;
    }
    printf("Years: %i", yrs);

    // TODO: Print number of years
}
