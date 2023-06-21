#include <cs50.h>
#include <stdio.h>
#include <math.h>


// prototypes
int no_of_digits(long c_num);
bool credit_card_validater(long c_num, int no_digits);
bool visa_validater(int num);
bool american_express_validater(int num);
bool mastercard_validater(int num);

int main(void)
{
    // get credit card number
    long card_number = get_long("Enter card number: ");

    // calculates no of digits ie 456 = 3
    int no_digits = no_of_digits(card_number);

    // validates the card number by luhn's algorithm
    bool card_validitity = credit_card_validater(card_number, no_digits);

    // first 2 numbers of the card
    long  div = pow(10, (no_digits - 2));
    int num = card_number / div;

    if (card_validitity)
    {
        // switchs depending on the no of digits
        switch (no_digits)
        {
            case 15:
                if (american_express_validater(num))
                {
                    printf("AMEX\n");
                    break;
                }

            case 13:
                if (visa_validater(num))
                {
                    printf("VISA\n");
                    break;
                }

            case 16:
                if (mastercard_validater(num))
                {
                    printf("MASTERCARD\n");
                    break;
                }
                else if (visa_validater(num))
                {
                    printf("VISA\n");
                    break;
                }

            default:
                printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

// calculates no of digits
int no_of_digits(long c_num)
{
    int cnt = 0;
    do
    {
        c_num /= 10;
        cnt++;
    }
    while (c_num > 0);
    return cnt;
}

// validates credit card number
bool credit_card_validater(long c_num, int no_digits)
{
    no_digits ++;
    int n, sum = 0;
    for (int i = 1 ; i < no_digits ; i++)
    {
        n = c_num % 10;
        if (i % 2 == 0)
        {
            n *= 2;
            if (n > 9)
            {
                sum ++;
                sum += n % 10;
            }
            else
            {
                sum += n;
            }
        }
        else
        {
            sum += n;
        }
        c_num /= 10;
    }
    if (sum % 10 == 0)
    {
        return true;
    }
    return false;
}

// validates if its a visa card
bool visa_validater(int num)
{

    if (num / 10 == 4)
    {
        return true;
    }
    return false;
}

// validates if its a amex card
bool american_express_validater(int num)
{
    if (num == 34 || num == 37)
    {
        return true;
    }
    return false;
}

// validates if its a mastercard card
bool mastercard_validater(int num)
{
    if (50 < num && num < 56)
    {
        return true;
    }
    return false;
}
