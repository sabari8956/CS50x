// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 17576; // 26^3

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO

    int index = hash(word);
    node *cursor = table[index];
    for (node *temp = cursor; temp != NULL; temp = temp->next)
    {
        // check if same
        if (strcasecmp(temp->word, word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int hashValue = 0;
    hashValue = (toupper(word[0]) - 'A') * 676;
    char l2 = word[1];

    if (l2 != '\0')
    {
        hashValue += (toupper(l2) - 'A') * 26;
    }

    char l3 = word[2];
    if (l3 != '\0')
    {
        hashValue += (toupper(l3) - 'A');
    }

    return hashValue;
}


unsigned int cnt = 0;

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }
    char word[LENGTH + 1];

    while (fscanf(dict, "%s", word) != EOF)
    {
        // keeping track of number of words added
        cnt ++;

        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            return false;
        }
        strcpy(new_node -> word, word);
        new_node -> next = NULL;

        // index to be added into the hash table

        unsigned int index_hash = hash(word);

        if (table[index_hash] == NULL)
        {
            table[index_hash] = new_node;
        }

        else
        {
            /*My idea was to add the new node at last of the linked
            list but it seems that it should be added in the front i would like see the speed diffrence between them
            being in front of the list and last , this could be faster if the test function isn't biased
            i'll  code both of them lets see  */

            // well i think adding in last might bee slower coz if there are more elements in the linked list i will have to traverse through all the
            // elements, it will be O(n)

            // node *ptr = table[index_hash] ;
            // while(ptr ->next != 0x0)
            // {
            //     ptr = ptr-> next;
            // }
            // ptr -> next = new_node;

            // when i run load with above code as expected/(usual) i was wrong
            // it took 0.08 just for load and over all it was 0.12

            // but if it was aadding it in the front it will be just O(1)
            // so now i get it why they wanted me to add the new node at front

            // adding new node at front of the linked list

            new_node -> next = table[index_hash];
            table[index_hash] = new_node;

            // when running with this it was just 0.02 in load and over all it was 0.07

            // it will add the new node in front of the linkedlist
        }
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return cnt;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    node *tmp = 0x0;
    node *ptr = 0x0;
    for (int i = 0; i < N; i++)
    {
        ptr = table[i];
        while (ptr != 0x0)
        {
            tmp = ptr;
            ptr = ptr -> next;
            free(tmp);
        }
    }
    return true;
}
