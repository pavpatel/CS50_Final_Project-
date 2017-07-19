#include <cs50.h>
#include <stdio.h>

int main(void) 
{
    printf("How many minutes did you shower? ");
    int x = get_int();
    
    printf("Your shower took %i minutes.\n", x * 12);
}