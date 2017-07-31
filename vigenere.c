#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int alphacheck(string k);

int main(int argc, string argv[])
{
   string k = argv[1];
   
   //Error message for insufficient arguements
   if (argc != 2)
   {
      printf("Usage: ./vigenere k\n");
      return 1;
   }
   //Error message for non-alpha arguement
   else if (alphacheck(k) != 0)  
   {
      printf("Invalid arguement, must supply string\n");
      return 1;
   }
   
   //Prompt user for plain text
   printf("plaintext: ");
   string p = get_string();
   printf("ciphertext: ");
   
   //Loop through plaintext and key characters, restart key loop when key > plaintext length through mod m
   for (int i = 0, j = 0, n = strlen(p), m = strlen(k); i < n; i++)
   {
      //Upper and lower case determination and conversion
      if (islower(p[i]))
      {
         printf("%c", ((((p[i] + toupper(k[(i - j) % m]) - 65) - 97) % 26) + 97));
      }
      else if (isupper(p[i]))
      {
         printf("%c",((((p[i] + toupper(k[(i - j) % m]) - 65) - 65) % 26) + 65));
      }
      //Index spaces in j to remove from plaintext index i and print non-alpha
      else
      {
         printf("%c", p[i]);
         j++;
      }
   
   }
   //Finish with new line print
   printf("\n");
   return 0;
}

//Loop through characters of key and check for alpha
int alphacheck(string k)
{
  for (int i = 0, n = strlen(k); i < n; i++)
  {
     if (!isalpha(k[i]))
     {
        return 1;
     }
  }
  return 0;
}
