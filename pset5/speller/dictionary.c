// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

#define HASHTABLE_SIZE 65536

//define struct node
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//declare integer word_count to count the total number of words in the dictionary
int word_count = 0;

// declare hashtable
node *hashtable[HASHTABLE_SIZE];

//hash funtion
//yangrq1018 https://gist.github.com/yangrq1018/e769abd7489ea8f52351b30829796a15
int hash(const char *word)
{
    unsigned int hash = 0;

    for (int i = 0, n = strlen(word); i < n; i++)
    {
        hash = (hash << 2) ^ word[i];
    }

    return hash % HASHTABLE_SIZE;
}


//Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //opens file dictionary
    FILE *dict = fopen(dictionary, "r");

    //if dictionary does not open or is empty, return error
    if (dict == NULL)
    {
        printf("File cannot be opened for reading.");
        return 1;
    }

    //declare variable buffer to set max number of letters in a word
    char buffer[LENGTH];
    
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        hashtable[i] = NULL;
    }

    //loop through every word in the dictionary
    while (fscanf(dict, "%s", buffer) != EOF)
    {
        //creates a temporary node to store the word from the dictionary
        node *w = calloc(1, sizeof(node));

        //if new node is empty, return error
        if (w == NULL)
        {
            return 1;
        }

        //copies characters in string to w -> word
        strncpy(w -> word, buffer, sizeof(buffer));

        //implement hash function to get the index of the word
        int index = hash(buffer);

        //if the corresponding index in hashtable is empty, assign it to the temp node
        if (hashtable[index] == NULL)
        {
            hashtable[index] = w;
        }

        //else append w to the start of the linked list
        else
        {
            w -> next = hashtable[index];
            hashtable[index] = w;
        }

        //increase word_count by 1
        word_count ++;

    }
    
    //close dictionary file
    fclose(dict);

    return true;
}

//Returns true if word is in dictionary else false
bool check(const char *word)
{
    //gets length of word
    int len = strlen(word);

    //creates string one more than length of word
    char lword[len + 1];

    //lowercases every char in the word
    for (int i = 0; i < len; i++)
    {
        lword[i] = tolower(word[i]);
    }

    //declares last value in string to null
    lword[len] = '\0';

    //declares value of bucket as hash value for word
    int bucket = hash(lword);

    //creates new node for finding the value in the hashtable
    node *cursor = hashtable[bucket];

    //if node is not NULL
    while (cursor != NULL)
    {
        //follows the trail until both words match or value reached is NULL
        if (strncasecmp(cursor -> word, lword, sizeof(lword)) == 0)
        {
            //printf("%s\n", cursor -> word);
            //printf("%s\n", lword);
            return true;
        }

        else
        {
            cursor = hashtable[bucket] -> next;

            hashtable[bucket] = cursor;
        }
    }

    return false;
}

//Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // for each node in the hashtable
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        // check the table for a node at that index
        node *cursor = hashtable[i];
        
        while (cursor != NULL)
        {
            // create a temporary node
            node *temp = cursor;
            
            cursor = cursor -> next;

            // free the current node
            free(temp);
        }
    }
    return true;
}