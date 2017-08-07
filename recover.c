#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

//Define block size 
#define BLOCK 512

//Set Byte as unsigned integer of 8 (bits) 
//typedef uint8_t  BYTE;


int main(int argc, char *argv[])
{
    
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover forensic_file\n");
        return 1;
    }
    
    // open input file 
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }
    
    //Create output file
    FILE *img = NULL;
    
    //Initialize buffer
    unsigned char buffer[BLOCK];
    
    //Initialize array to store jpg filenames
    char filename[8];
    
    //Initialize to track jpgs for filenaming 
    int jpg = 0;
        
    //for (int i = 0; i < BLOCK; i++)
    //{
    
    //Read forsensic file's data
    //fread(&buffer, 1, BLOCK, file); //Read 1 block of 512 Bytes/ea
    //fread(&buffer, BLOCK, 1, file); //Read 512 blocks of 1 Byte/ea
        
    //}
    
    //Loop until end of file is reached
    //while (!feof(file))
    
    //Loop until 512 Bytes no longer read, signifying end of file
    while (fread(&buffer, BLOCK, 1, file) == 1)
    {
        //Read forensic file (REMOVED because redundant)
        //fread(&buffer, BLOCK, 1, file); //Read 512 blocks of 1 Byte/ea
        
        //Check first four bytes for jpg signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //Close output file from previous jpg
            if (img != NULL)
            {
                fclose(img);
            }
            
            //Print filename with jpg number in ### format
            sprintf(filename, "%03i.jpg", jpg);
            
            //Open output file in write mode
            img = fopen(filename, "w");
            
            //Write jpg to output file 512 bytes at a time 
            fwrite(&buffer, BLOCK, 1, img);
            
            //Increment for next jpg filename
            jpg++;
        }
        
        //Write next 512 Bytes of current jpg, to current file, until next jpg is detected
        else if (jpg > 0)
        {
            fwrite(&buffer, BLOCK, 1, img);     
        }
    }
    //Close input and output files
    fclose(file);
    fclose(img);
    
    return 0;
    
}