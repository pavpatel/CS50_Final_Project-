#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
   //Error message for insufficient arguements
   if (argc != 2)
   {
       printf("Usage: ./caesar k\n");
       return 1;
   }

   //Convert key from string to int
   int k = atoi(argv[1]);

   //Error message for negative integer key
   if (k < 0)
   {
       printf("Key must be a non-negative integer.\n");
       return 1;
   }

   //Prompt user for plain text
   printf("plaintext: ");
   string p = get_string();
   printf("ciphertext: ");

   //Loop through characters
   for (int i = 0, n = strlen(p); i < n; i++)
   {
      //Upper and lower case determination and conversion, print non-alpha
      if (islower(p[i]))
      {
         printf("%c", ((((p[i] + k) - 97) % 26) + 97));
      }
      else if (isupper(p[i]))
      {
         printf("%c", ((((p[i] + k) - 65) % 26) + 65));
      }
      else
      {
         printf("%c", p[i]);
      }

   }

   //Finish with new line print
   printf("\n");
   return 0;
}
