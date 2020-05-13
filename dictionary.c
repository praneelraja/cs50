// Implements a dictionary's functionality

#include <stdbool.h>
#include <strings.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

// size of Hash table
int size_h = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int index = hash(word);
    node *temp = table[index];
    while(temp)
    {
        if(!strcasecmp(temp->word,word))
            return true;
        temp = temp->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    int hashv = 0;
    int i = 0;
    // alternate way
    // int j = LENGTH / 2; as lenght is too big
    // hashv = hashv + (tolower(word[i]) - ('a' - 1)) * j);
    // j--; in while loop
    int j = 26;

    while(word[i] != '\0' && j > 0)
    {
        //hashv = hashv + (tolower(word[i]) - ('a' - 1)) * (i + 1);
        hashv = hashv + (tolower(word[i]) - ('a' - 1)) * j;
        j--;
        i++;
    }
    hashv = hashv % N;
    return hashv;
}



bool insert(const char *word)
{
    //create a node;
    node *nptr = malloc(sizeof(node));

    //check if node was created
    if(nptr == NULL)
        return false;

    //fill node with data
    //char *temp = nptr->word;
    int i = 0;
    while(word[i] != '\0')
    {
        nptr->word[i] = word[i];
        i++;
    }
    nptr->word[i] = '\0';

    int index = hash(word);

    //make the new node point to  whatever hash table array is pointing to.
    nptr->next = table[index];
    table[index] = nptr;

    size_h++;
    return true;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //get word from dictionary in to this
    char word[LENGTH + 1];

    //initialize hashtable to NULL so that we can be sure if the value is not NULL then it points to a list and not garbage
    for(int i = 0; i < N; i++)
        table[i] = NULL;

    // Open dictionary
    FILE *fp = fopen(dictionary, "r");
    if(fp == NULL)
    {
        return false;
    }

    while(EOF != fscanf(fp,"%s",word))
    {
        if(!insert(word))
        {
            printf("Could not allocate memory for word %s in %s. So skipping it\n", word, dictionary);
        }
    }

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // returns the size of global variable
    return size_h;
}

bool remove_list(node *del)
{
    node *temp = del;
    while(temp)
    {
        del = del->next;
        free(temp);
        size_h--;
        temp = del;
    }
    return true;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for(int i = 0; i < N && size_h > 0 ; i++)
    {
        if(table[i] != NULL)
        {
            remove_list(table[i]);
            //bool suc = remove_list(table[i]);
            //if(!suc)
              //  return false;
        }
    }

    return true;
}
