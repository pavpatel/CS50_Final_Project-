#include <cs50.h>
#include <stdio.h>

int main(void) 
{
   int height;
   int hashes;
   int spaces;
   int rows;
   
   do
   {
        printf("Enter a number between 0 and 23 ");
        height = get_int();
   } 
   while (height < 0 || height > 23);
   
   for (rows = 0; rows < height; rows++)
   {
       for (spaces = (height - 2); spaces > rows - 1; spaces--)
       {
           printf(" ");
       }
       
       for (hashes = (height - spaces) - 1; hashes <= (height + 1); hashes++)
       //(hashes = (rows +1; hashes <= (height + 1); hashes++)
       {
           printf("#");
       }
   
       printf("\n");
   }
   
}