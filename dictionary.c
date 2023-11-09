// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
const unsigned int N = 2000;

// Hash table
node *table[N];

// Count de number of nodes
int total_nodes = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Cria ponteiro que aponta para casa posição na lista encadeada
    node *cursor = table[hash(word)];
    // Percorre a lista no slot apontado pela função hash, retorna true se a posição do cursor for igual a entrada na função
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
            return true;
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Soma os valores encontrados na palavra
    int sum = 0;
    int letra;
    for(int i = 0; i < strlen(word); i++)
    {
        // Soma o valor de todas as letras da palavra
        letra = tolower(word[i]);
        sum += letra;
    }
    // A escolha de números 31 é à gosto do programador, e o % N garante que os resultados vão ser entre 0 e N;
    int result = (sum * 31) % N;
    return result;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the file
    FILE *dicionario = fopen(dictionary, "r");
    if (dicionario == NULL)
        return false;

    // Sets all next pointers in hashtable to 0
    for (int i = 0; i < N; i++)
        table[i] = NULL;

    // Cria um buffer para armazenar cada letra do dicionario por vez
    char buffer_word[LENGTH + 1];
    while (fscanf(dicionario, "%s", buffer_word) != EOF)
    {
        // A função hash indica em qual slot da hashtable a palavra deve ir
        int position_table = hash(buffer_word);

        // Aloca dinamicamente um espaço para um novo node de uma lista encadeada
        node *new = malloc(sizeof(node));
        if (new == NULL)
            return false;

        // A variável global que conta o numero de palavras do dicionário adicionadas, recebe mais um. Será usada posteriormente na função size
        total_nodes++;

        // Usa a função strcopy para copiar o conteudo do buffer para o novo node
        strcpy(new->word, buffer_word);

        // O item next do node aponta para a cabeça da linked list (último node criado)
        new->next = table[position_table];

        // A cabeça da lista aponta para o node recem criado
        table[position_table] = new;
    }
    // Sucesso
    fclose(dicionario);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return total_nodes;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Percorre a lista com um cursor e uma variavel temporaria que segue o cursor, evitando de perder o acesso a toda a lista.
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *last_address = cursor;
            cursor = cursor->next;
            free(last_address);
        }
        free(cursor);
    }
    return true;
}
