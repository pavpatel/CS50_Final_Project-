/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */

//Binary Search
bool search(int value, int values[], int n)
{
    // Returns false if non-positive integer supplied
    if (value < 1) 
    {
        return false;    
    }
    
    //Initialize min and max indices 
    int min = 0;
    int max = n - 1;
    
    //Repeat until all elements have been parsed
    do
    {
        //Set midpoint
        int mid = (min + max) / 2;
        
        //Return true if needle found
        if (value == values[mid])
        {
            return true;
        }
        
        //Lower max if needle is less than midpoint
        else if (value < values[mid]) 
        {
            max = mid - 1;
        }
        //Increase min if needle is greater than midpoint
        else if (value > values[mid])
        {
            min = mid + 1;
        }
        
        //Count remaining elements to parse for each loop
        n = (max - min) + 1;
    }
    while (n > 0);
    
    //Return false if needle is not in haystack
    return false;
    
    /*//Loop through haystack and return true if needle is found (LINEAR)
    for (int i = 0; i < n; i++)
    {
        if (values[i] == value)
        {
            return true;
        }
    }
    return false;*/
}

/**
 * Sorts array of n values.
 */

//Bubble Sort
void sort(int values[], int n)
{
    //Initialize swap at non-zero value
    int swaps = -1;
   
    //Repeat until no swaps
    do 
    {
        //Reset swaps to zero
        swaps = 0;
        
        //Loop through haystack, sort adjacent elements, count swaps
        for (int i = 0, min = 0; i < n - 1; i++)
        {
            if (values[i] > values[i + 1])
            {
                min = values[i + 1];
                values[i + 1] = values[i];
                values[i] = min;
                swaps++;
            }
        }
    }
    while (swaps > 0);
    return;
}
