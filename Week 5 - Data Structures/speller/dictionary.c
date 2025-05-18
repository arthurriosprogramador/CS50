// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

const unsigned int N = 1000;

// Hash table
node *table[N];

// Word count
unsigned int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char temp[LENGTH + 1];
    int length = strlen(word);

    for (int i = 0; i < length; i++)
    {
        temp[i] = tolower(word[i]);
    }

    temp[length] = '\0';

    unsigned int index = hash(temp);

    node *p = table[index];
    while (p != NULL)
    {
        if (strcmp(p->word, temp) == 0)
        {
            return true;
        }
        p = p->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int sum = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        sum += tolower(word[i]);
    }

    return sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];

    // Read each word in the file
    while (fscanf(source, "%s", word) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            fclose(source);
            return false;
        }

        strcpy(new_node->word, word);
        new_node->next = NULL;

        unsigned int index = hash(word);

        new_node->next = table[index];
        table[index] = new_node;

        word_count++;
    }

    // Close the dictionary file
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *p = table[i];
        while (p != NULL)
        {
            node *temp = p;
            p = p->next;
            free(temp);
        }
    }

    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    return true;
}
