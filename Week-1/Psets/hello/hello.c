#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Gets name
    string name = get_string("Enter your name: ");
    // prints the name
    printf("hello, %s\n", name);
}