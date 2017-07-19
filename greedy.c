#include <cs50.h>
#include <stdio.h>
#include <math.h>


int main(void) 
{

    float amt;
    
    do
    {
    printf("O hai! How much change is owed? $ ");
    amt = get_float();
    }
    while (amt < 0);
    
    amt = round(amt * 100);
    
    int cents = amt;
    int quarter = cents / 25;
    int quart_mod = cents % 25;
    int dime = quart_mod / 10;
    int dime_mod = quart_mod % 10;
    int nickel = dime_mod / 5;
    int nickel_mod = dime_mod % 5;
    int penny = nickel_mod / 1;
    
    printf("%i\n", quarter + dime + nickel + penny);

}