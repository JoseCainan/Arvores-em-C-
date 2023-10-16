#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 50
#define MAX_NAME_LENGTH 50

typedef struct
{
    int key;
    char name[MAX_NAME_LENGTH];
    char job[MAX_NAME_LENGTH];
    float monthlyIncome;
    int isActive;
} Entry;

Entry table[TABLE_SIZE];

void initializeTable()
{
    int i;
    for (i = 0; i < TABLE_SIZE; i++)
    {
        table[i].key = -1;
        table[i].isActive = 0;
    }
}

int hashFunction(int x, int k)
{
    int h0 = x % 47;
    int h1 = x % 43;
    return (h0 + k * h1) % TABLE_SIZE;
}

void insert(int key, const char *name, const char *job, float monthlyIncome)
{
    int k = 0;
    int index = hashFunction(key, k);

    while (table[index].isActive)
    {
        k++;
        index = hashFunction(key, k);
    }

    table[index].key = key;
    strcpy(table[index].name, name);
    strcpy(table[index].job, job);
    table[index].monthlyIncome = monthlyIncome;
    table[index].isActive = 1;
}

void printEntry(const Entry *entry)
{
    printf("Chave: %d\n", entry->key);
    printf("Nome: %s\n", entry->name);
    printf("Emprego: %s\n", entry->job);
    printf("Renda Mensal: %.2f\n", entry->monthlyIncome);
}

int search(int key)
{
    int k = 0;
    int index = hashFunction(key, k);

    while (table[index].key != key && table[index].isActive)
    {
        k++;
        index = hashFunction(key, k);
    }

    if (table[index].key == key)
    {
        return index;
    }

    return -1; // Chave não encontrada
}

int main()
{
    initializeTable();

    // Exemplo de inserção de dados
    insert(1, "João", "Engenheiro", 5000.0);
    insert(2, "Maria", "Advogada", 7000.0);
    insert(3, "Carlos", "Professor", 4000.0);

    // Exemplo de busca
    int index = search(2);
    if (index != -1)
    {
        printf("Participante encontrado:\n");
        printEntry(&table[index]);
    }
    else
    {
        printf("Participante não encontrado\n");
    }

    return 0;
}
