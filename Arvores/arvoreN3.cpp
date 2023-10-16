/*Gerenciamento de passagens de aviao utilizando arvore*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
  int localizador;
  char data[10];
  float preco;
  char destino[50];
} passagem;

typedef struct node
{
  passagem passagem;
  struct node *esquerda;
  struct node *direita;
} No;

int quant_voos = NULL;

// raizes pras arvores
No *root_br = NULL;
No *root_ar = NULL;
No *root_consolidada = NULL;

int loc()
{
  int loc = rand() % 100; // gerando um localizadorero aleatorio
  while (loc == 0)        // evitando q o localizador seja 0
  {
    loc = rand() % 100;
  }
  return loc;
}

int buscar_localizador(No *node, int localizador)
{

  if (node == NULL) // caso o nó seja nulo, não há oq testar
  {
    return 0;
  }

  if (node->passagem.localizador == localizador)
  {
    return 1; 
  }

  // recursividade para procurar nas subarvores
  buscar_localizador(node->direita, localizador);
  buscar_localizador(node->esquerda, localizador);

  return 0;
}

int gerar_localizador()
{
  int localizador = loc(); // recebe o valor gerado pela função loc
  while ((buscar_localizador(root_br, localizador) == 1 || buscar_localizador(root_ar, localizador) == 1))
  { // equanto a função busca_localizador retornar 1 para qualquer uma das arvores será gerado um novo localizador
    localizador = loc();
  }
  return localizador;
}

No *criar_no()
{

  No *node = (No *)malloc(sizeof(No));

  printf("Insira a data da viagem (use o formato DD/MM/AAAA): ");
  scanf("%s", node->passagem.data);
  fflush(stdin);
  printf("Insira o preço da passagem: ");
  scanf("%f", &node->passagem.preco);
  fflush(stdin);
  printf("Insira o destino: ");
  scanf("%50[^\n]s", node->passagem.destino);
  fflush(stdin);

  node->passagem.localizador = gerar_localizador(); // o campo localizador do nó recebe o valor retornado pela função gerar_localizador

  node->esquerda = NULL;
  node->direita = NULL;

  return node;
}

void inserir_no(No *node, No *novo)
{
  if (node == NULL)
  {
    printf("Erro\n");
    return;
  }

  if (node->passagem.localizador < novo->passagem.localizador)
  { // caso o localizador do nó a ser inserido seja maior que o localizador do nó que está na raiz
    // o nó a ser inserido irá para a direita da raiz

    if (node->direita == NULL) // testando se o nó da direita está nulo
    {                      // caso esteja, o nó novo sera inserido aqui
      node->direita = novo;
      return;
    }
    inserir_no(node->direita, novo); // caso nao esteja nulo, chamará recursivamente a função de inserir nó
                                 // para continuar procurando onde inserir o nó
  }
  else
  { // caso o localizador do nó a ser inserido seja menor que o localizador do nó que está na raiz
    // o nó a ser inserido irá para a esquerda da raiz
    if (node->esquerda == NULL)
    {
      node->esquerda = novo;
      return;
    }
    inserir_no(node->esquerda, novo);
  }
}


void imprimir(No *node) // função para imprimir a arvore
{
  if (node == NULL)
  {
    return;
  }
  printf("Destino: %s -> ", node->passagem.destino);
  printf("Preco: %.2f reais -> ", node->passagem.preco);
  printf("Data: %s -> ", node->passagem.data);
  printf("Localizador: %d\n", node->passagem.localizador);
  printf("\n");
  imprimir(node->direita);
  imprimir(node->esquerda);
}

void imprimir_folha(No *node) // usada na busca por destino
{
  printf("Destino: %s -> ", node->passagem.destino);
  printf("Preço: %.2f reais -> ", node->passagem.preco);
  printf("Data: %s -> ", node->passagem.data);
  printf("Localizador: %d\n", node->passagem.localizador);
}

void inserir_raiz(No **raiz) 
{

  if ((*raiz) == NULL) // se a raiz for nula, ja insere a nova folha na raiz e encerra a função
  {
    *raiz = criar_no();
    return;
  }
  // caso a raiz nao seja nula, irá criar um novo nó e chamar a função inserir nó;
  No *novo = criar_no();
  inserir_no(*raiz, novo);
}

No *remover(No *node, int num);

No *buscar_pai(No *node, int num) {
  if (node == NULL)
    return NULL;

  if ((node->esquerda != NULL) && (node->esquerda->passagem.localizador == num)) {
    return node;
  }

  if ((node->direita != NULL) && (node->direita->passagem.localizador == num)) {
    return node;
  }

  if (node->passagem.localizador < num) {
    return buscar_pai(node->direita, num);
  } else {
    return buscar_pai(node->esquerda, num);
  }
}

bool um_filho(No *node) {
  // return (node->esq == NULL) && (node->dir == NULL);
  if ((node->esquerda == NULL) && (node->direita != NULL)) {
    return true;
  }

  if ((node->esquerda != NULL) && (node->direita == NULL)) {
    return true;
  }

  return false;
}

bool eh_folha(No *node) {
  // return (node->esq == NULL) && (node->dir == NULL);
  if ((node->esquerda == NULL) && (node->direita == NULL)) {
    return true;
  } else {
    return false;
  }
}

No *maior(No *node) {
  if (node == NULL)
    return NULL;
  if (node->direita == NULL)
    return node;
  return maior(node->direita);
}

void substituir(No *pai, No *removido) {
  if (um_filho(removido)) {
    if (removido->esquerda!= NULL) {
      pai->esquerda = removido->esquerda;
    } else {
      pai->direita = removido->direita;
    }
  } else {
    No *substituto = maior(removido->esquerda);
    remover(pai, substituto->passagem.localizador);
    substituto->esquerda = removido->esquerda;
    substituto->direita = removido->direita;
    if (pai->passagem.localizador < substituto->passagem.localizador) {
      pai->direita = substituto;
    } else {
      pai->esquerda = substituto;
    }
    removido->direita = NULL;
    removido->esquerda = NULL;
  }
}

No *remover(No *node, int num) {
  No *pai = buscar_pai(node, num);
  if (pai == NULL) {
    printf("Numero não encontrado");
    return NULL;
  } else {
    // printf("Pai: %d\n", pai->num);
    No *removido = NULL;
    if (pai->passagem.localizador < num) {
      removido = pai->direita;
      if (eh_folha(removido)) {
        pai->direita = NULL;
      } else {
        substituir(pai, removido);
      }
    } else {
      removido = pai->esquerda;
      if (eh_folha(removido)) {
        pai->esquerda = NULL;
      } else {
        substituir(pai, removido);
      }
    }
    // printf("Removido: %d\n", removido->num);
    return removido;
  }
}

No *remover_raiz(No **raiz) {

  if (*raiz == NULL) {
    printf("Árvore vazia\n");
    return NULL ;
  }

  int num = (*raiz)->passagem.localizador; //verifica se o localizador é um numero
   
   No *remove = *raiz;
  if ((*raiz)->passagem.localizador == num) {

    if (eh_folha(remove)) { //verifica se o no nao tem filho
      *raiz = NULL;
      return remove;
    }

    if (um_filho(remove)) {//verifica se é um filho
      
      if ((*raiz)->esquerda != NULL) {
        *raiz = (*raiz)->esquerda;
        remove->esquerda = NULL;
      } else {
        *raiz = (*raiz)->direita;
        remove->direita = NULL;
      } 
      return remove;
     

    } else {
      No *substituto = maior((*raiz)->esquerda);
      remover(*raiz, substituto->passagem.localizador);
      substituto->esquerda = (*raiz)->esquerda;
      substituto->direita = (*raiz)->direita;
      (*raiz)->esquerda = NULL;
      (*raiz)->direita = NULL;
      remove = *raiz;
      *raiz = substituto;
      return remove;
    
    }
  } else {
    remover(*raiz, num);
  }
}

void preencher_consolidada(No **raiz, No *node) {
  if (*raiz == NULL) {
    *raiz = node;
    return;
  }

  if ((*raiz)->passagem.localizador < node->passagem.localizador) {
    if ((*raiz)->direita == NULL) {
      (*raiz)->direita = node;
      return;
    }
    preencher_consolidada(&(*raiz)->direita, node);
  } else {
    if ((*raiz)->esquerda == NULL) {
      (*raiz)->esquerda = node;
      return;
    }
    preencher_consolidada(&(*raiz)->esquerda, node);
  }
}

void inserir_consolidada(No **root, No *node)
{
  // metodo de inserir para a arvore consolidada, o teste da raiz nula está aqui dentro desse metodo
  if (*root == NULL)
  {
    *root = node;
    return;
  }

  if ((*root)->passagem.localizador < node->passagem.localizador)
  {
    if ((*root)->direita == NULL)
    {
      (*root)->direita = node;
      return;
    }
    inserir_consolidada(&(*root)->direita, node);
  }
  else
  {
    if ((*root)->esquerda == NULL)
    {
      (*root)->esquerda = node;
      return;
    }
    inserir_consolidada(&(*root)->esquerda, node);
  }
}

int aux=0;
void buscar_destino(No *node, char *destino)
{
   
  if (node == NULL) // caso o nó seja nulo, retorna
  {
    return;
  }

  if (strcmp(node->passagem.destino, destino) == 0) 
  {
   aux++;
  }

  buscar_destino(node->direita, destino);
  buscar_destino(node->esquerda, destino);

  

}

void menu()
{
  printf("\t\n|------MENU-----|\n");
  printf("\t| 1-Inserir Passagem para Brasil               |\n");
  printf("\t| 2-Inserir Passagem para Argentina            |\n");
  printf("\t| 3-Buscar Passagem para Brasil                |\n");
  printf("\t| 4-Buscar Passagem para Argentina             |\n");
  printf("\t| 5-Imprimir Arvore Brasil                     |\n");
  printf("\t| 6-Imprimir Arvore Argentina                  |\n");
  printf("\t| 7-Valores das passagens vendidas do Brasil   |\n");
  printf("\t| 8-Valores das passagens vendidas da Argentina|\n");
  printf("\t| 9-Valores de passagens vendidas Br e Ar      |\n");
  printf("\t| 10-Consolidar Vendas                         |\n");
  printf("\t| 11-Imprimir Arvore Consolidada               |\n");
  printf("\t| 0-Sair                                       |\n");
}

float total_pass = 0;
int soma_pass(No *node){
  
  if(node == NULL) return NULL;

  total_pass = node->passagem.preco + total_pass;
   soma_pass(node->direita);
   soma_pass(node->esquerda);

   return total_pass;

}

void consolidar(No **raiz) {

  while ((*raiz) != NULL) {
   preencher_consolidada( &root_consolidada,remover_raiz(raiz));
  }
}

int main()
{
  srand(time(NULL)); 
 float total = 0;
 float preco_br=NULL;
 float preco_ar=NULL;
 float preco_total = NULL;
 
  int opc = 1;
  char destino[50];

  while (opc != 0)
  {
    menu();
    scanf("%d", &opc);
    fflush(stdin);
    switch (opc)
    {
    case 1:
      inserir_raiz(&root_br);
       quant_voos++;
      
      break;

    case 2:
      inserir_raiz(&root_ar);
      quant_voos++;
      
      break;

    case 3:
      printf("Digite o destino a ser procurado: ");
      scanf("%50[^\n]s", destino);
      buscar_destino(root_br, destino);
      break;

    case 4:
      printf("Digite o destino a ser procurado: ");
      scanf("%50[^\n]s", destino);
      aux=0;
      buscar_destino(root_ar, destino);
      printf("a quantidade é; %d",aux);
      
      break;

    case 5:
      imprimir(root_br);
      break;

    case 6:
      imprimir(root_ar);
      break;

    case 7:
    total_pass = 0;
    preco_br = soma_pass(root_br);
    printf("O total dos precos das passagens vendidas : %.2f\n", preco_br);
    preco_br = 0;
      
      break;

    case 8:
    total_pass= 0;
    preco_ar = soma_pass(root_ar);
    printf("O total dos precos das passagens vendidas : %.2f\n", preco_ar);
    preco_ar=0;
      break;

    case 9:
    
      preco_total = preco_br + preco_ar;
      printf("Total dos valores das passagens %.2f\n", preco_total);
      preco_ar= 0;
      preco_br = 0;
      
      break;

    case 10:
      // a funcao consolidar só realiza o processo para uma arvore de cada vez
      consolidar(&root_br);
      consolidar(&root_ar);
      
      break;

    case 11:
      imprimir(root_consolidada);
      break;
    
    case 0:
      break;

    default:
      printf("Opção inválida\n");
      break;
    }
  }

  return 0;
}