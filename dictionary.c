/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>

#include "dictionary.h"


//Define struct for nodes with pointer value
typedef struct node 
{
    char word[LENGTH + 1];
    struct node* next;
} 
node; 

//Create hash table
node *hashtable[HASHTABLE_SIZE];

//Initialize to track words loaded from dictionary
int wordcount;

/* Hash function
 * Source: https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/cf9nlkn/
*/

int hash_it(char* word)
{
    unsigned int hash = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
        hash = (hash << 2) ^ word[i];
    return hash % HASHTABLE_SIZE;
}


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    //Initialize array for copy of word to normalize for search (const = read only) 
    int len = strlen(word);
    char low_word[len + 1];
    
    //Loop through word characters and make lower
    for (int i = 0; i < len; i++)
    {
        low_word[i] = tolower(word[i]);
    }
    
    //Add string terminator
    low_word[len] = '\0';
    
    //Generate hash table index to search
    int l_idx = hash_it(low_word);
    
    //Initialize search node
    node *search = hashtable[l_idx];
    
    //Loop through all nodes of linked list
    while (search != NULL)
    {
        //If word is found return true otherwise check next
        if (strcmp(search->word, low_word) == 0)
        {
            return true;
        }
        else
        {
            search = search->next;
        }
    }
    //Return false if word was not found
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    //Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }
    
    //Buffer to store words
    char wordbuff[LENGTH + 1];
    
    //Scan dictionary word by word until end of file is reached
    while(fscanf(file, "%s", wordbuff) != EOF)
    {
        //Malloc a node * for each word
        node *new_node = malloc(sizeof(node));
        
        //Error message if out of memory, as malloc will return NULL, and unload dictionary
        if (new_node == NULL)
        {
            fprintf(stderr, "Could not create new node, check allocated memory.\n");
            unload();
            return false;
        }
        //Copy scanned word into new_node; initialize new_node->next to remove garbage data 
        strcpy(new_node->word, wordbuff);
        new_node->next = NULL;
        
        //Call hashing function and store generated index
        int idx = hash_it(new_node->word);
        
        //Set head of linked list for this hash index
        //node *head = hashtable[idx];
        
        //If hashtable index is empty then assign new_node to index
        if (hashtable[idx] == NULL)
        {
            hashtable[idx] = new_node;
        }
        //Otherwise insert new_node to head of list; point new_node next to previous head; make new_node the head
        else 
        {
            //new_node->next = head;
            //head = new_node;
            new_node->next = hashtable[idx];
            hashtable[idx] = new_node;
        }
        wordcount++;
    }
    fclose(file);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (wordcount > 0 )
    {
        return wordcount;
    }
    else 
    {
        return 0; 
    }

}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    //Loop hash table indices 
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        //Find head of index
        node *head = hashtable[i];
        
        //Loop all nodes in linked list of hash index
        while (head != NULL)
        {
            //Create node to maintain linked list chain prior to head deletion
            node *del_head = head->next;
            
            //Free memory in head
            free(head);
            
            //Reset head 
            head = del_head;
        }
    }
    return true;
}
