#include <stdio.h>
#include <cs50.h>
#include <string.h> 
#include <ctype.h>

int main(void)
{
    string name = get_string();
    if (name != NULL)
    {
        //Print first letter of string
        printf("%c", toupper(name[0]));
        
        //Loop through all characters of string
        for (int i = 0, n = strlen(name); i < n; i++)
        {
            //Find space in string and print next character
            if(name[i] == ' ')
            {
                printf("%c", toupper(name[i + 1]));
            }
        }
    }
    printf("\n");
}
