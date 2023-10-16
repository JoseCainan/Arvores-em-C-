/* gerenciamento de uma farmacia utilizando arvores*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define T 5

typedef struct
{
    int chave;
    char nome_medicamento[50];
    char data_cadastro[11];
    int quantidade;
    char codigo[20];
    float preco;
} Registro;

typedef struct Pagina
{
    int n_chaves;
    int chaves[2 * T - 1];
    Registro registros[2 * T - 1];
    struct Pagina *filhos[2 * T];
    struct Pagina *prox; // Ponteiro para a próxima folha
    bool folha;
} Pagina;

Pagina *raiz = NULL;

Pagina *criar_pagina()
{
    Pagina *pag = (Pagina *)malloc(sizeof(Pagina));
    pag->n_chaves = 0;
    pag->folha = true;
    pag->prox = NULL;
    for (int i = 0; i < 2 * T; i++)
    {
        pag->filhos[i] = NULL;
    }
    return pag;
}

void dividir_pagina(Pagina *pai, int indice_filho, Pagina *filho)
{
    Pagina *nova_pag = criar_pagina();
    nova_pag->folha = filho->folha;
    nova_pag->n_chaves = T - 1;

    for (int i = 0; i < T - 1; i++)
    {
        nova_pag->chaves[i] = filho->chaves[i + T];
        nova_pag->registros[i] = filho->registros[i + T];
    }

    if (!filho->folha)
    {
        for (int i = 0; i < T; i++)
        {
            nova_pag->filhos[i] = filho->filhos[i + T];
            filho->filhos[i + T] = NULL;
        }
    }

    for (int i = pai->n_chaves; i > indice_filho; i--)
    {
        pai->filhos[i + 1] = pai->filhos[i];
    }
    pai->filhos[indice_filho + 1] = nova_pag;

    for (int i = pai->n_chaves - 1; i >= indice_filho; i--)
    {
        pai->chaves[i + 1] = pai->chaves[i];
        pai->registros[i + 1] = pai->registros[i];
    }
    pai->chaves[indice_filho] = filho->chaves[T - 1];
    pai->registros[indice_filho] = filho->registros[T - 1];
    pai->n_chaves++;
}

void inserir_chave_nao_cheia(Pagina *pag, int chave, Registro *registro)
{
    int indice = pag->n_chaves - 1;

    if (pag->folha)
    {
        while (indice >= 0 && chave < pag->chaves[indice])
        {
            pag->chaves[indice + 1] = pag->chaves[indice];
            pag->registros[indice + 1] = pag->registros[indice];
            indice--;
        }
        pag->chaves[indice + 1] = chave;
        pag->registros[indice + 1] = *registro;
        pag->n_chaves++;

        if (pag->prox != NULL) // Atualiza o encadeamento das folhas
        {
            pag->prox->prox = pag->prox;
            pag->prox = NULL;
        }
    }
    else
    {
        while (indice >= 0 && chave < pag->chaves[indice])
        {
            indice--;
        }
        indice++;

        if (pag->filhos[indice]->n_chaves == 2 * T - 1)
        {
            dividir_pagina(pag, indice, pag->filhos[indice]);
            if (chave > pag->chaves[indice])
            {
                indice++;
            }
        }
        inserir_chave_nao_cheia(pag->filhos[indice], chave, registro);
    }
}

void Busca_med(Pagina *pag, const char *nome)
{
    if (pag != NULL)
    {
        int i;
        for (i = 0; i < pag->n_chaves; i++)
        {
            if (!pag->folha)
            {
                Busca_med(pag->filhos[i], nome);
            }
            if (strcmp(pag->registros[i].nome_medicamento, nome) == 0)
            {
                printf("Medicamento encontrado:\n");
                printf("Chave: %d\n", pag->chaves[i]);
                printf("Nome do medicamento: %s\n", pag->registros[i].nome_medicamento);
                printf("Data do cadastro: %s\n", pag->registros[i].data_cadastro);
                printf("Quantidade: %d\n", pag->registros[i].quantidade);
                printf("Código: %s\n", pag->registros[i].codigo);
                printf("Preço: %.2f\n", pag->registros[i].preco);
                return;
            }
        }
        if (!pag->folha)
        {
            Busca_med(pag->filhos[i], nome);
        }
    }
    printf("Medicamento não encontrado.\n");
}

void inserir(int chave, Registro *registro)
{
    if (raiz == NULL)
    {
        raiz = criar_pagina();
    }

    if (raiz->n_chaves == 2 * T - 1)
    {
        Pagina *nova_raiz = criar_pagina();
        nova_raiz->folha = false;
        nova_raiz->filhos[0] = raiz;
        raiz = nova_raiz;
        dividir_pagina(raiz, 0, raiz->filhos[0]);
    }

    inserir_chave_nao_cheia(raiz, chave, registro);
}

void imprimir(Pagina *pag)
{
    if (pag != NULL)
    {
        int i;
        for (i = 0; i < pag->n_chaves; i++)
        {
            if (!pag->folha)
            {
                imprimir(pag->filhos[i]);
            }
            printf("Chave: %d\n", pag->chaves[i]);
            printf("Nome do medicamento: %s\n", pag->registros[i].nome_medicamento);
            printf("Data do cadastro: %s\n", pag->registros[i].data_cadastro);
            printf("Quantidade: %d\n", pag->registros[i].quantidade);
            printf("Código: %s\n", pag->registros[i].codigo);
            printf("Preço: %.2f\n", pag->registros[i].preco);
            printf("-----------------------------\n");
        }
        if (!pag->folha)
        {
            imprimir(pag->filhos[i]);
        }
    }
}

void receber_med(Registro *registro)
{
    printf("Digite o nome do medicamento: ");
    scanf(" %[^\n]%*c", registro->nome_medicamento);
    printf("Digite a data do cadastro (formato: DD/MM/AAAA): ");
    scanf(" %[^\n]%*c", registro->data_cadastro);
    printf("Digite a quantidade: ");
    scanf("%d", &(registro->quantidade));
    printf("Digite o código: ");
    scanf(" %[^\n]%*c", registro->codigo);
    printf("Digite o preço: ");
    scanf("%f", &(registro->preco));
}

int main()
{
    int opcao;
    int chave;
    char nome[50];
    Registro registro;

    while (opcao != -1)
    {
        printf("\n----- Farmacia do LULU %d -----\n", T);
        printf("|1- Inserir           |\n");
        printf("|2- Imprimir          |\n");
        printf("|3- Buscar medicamento|\n");
        printf("|4- Sair              |\n");
        printf("|O que deseja fazer? |-> ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("Digite a chave: ");
            scanf("%d", &chave);
            receber_med(&registro);
            inserir(chave, &registro);
            break;
        case 2:
            printf("\n----- Registros da farmacia -----\n");
            imprimir(raiz);
            break;
        case 3:
            printf("Informe o nome do medicamento: ");
            scanf(" %[^\n]%*c", nome);
            Busca_med(raiz, nome);
            break;
        case 4:
            printf("Encerrando o programa...\n");
            opcao = -1;
            break;
        default:
            printf("Opção inválida!\n");
            break;
        }
        getchar();
    }

    return 0;
}
