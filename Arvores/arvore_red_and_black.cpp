/* Arvore vermelho e preto */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

enum cor{
    preto = 1,
    vermelho = 0
};

typedef struct node
{

    cor cor = vermelho; // 0 vermelho 1 preto
    int valor;
    struct node *direita;
    struct node *esquerda;
    struct node *pai;

} Node;

typedef struct tree
{
    Node *raiz;

} Tree;

Node *busca_pai(Node *no, int aux){
    if(no == NULL) return NULL;

    if((no->esquerda != NULL) and (no->esquerda->valor == aux)) return no;

    if((no->direita != NULL) and (no->direita->valor == aux)) return no;

    if(no->valor < aux){
        return busca_pai(no->direita, aux);
    }else{
        return busca_pai(no->esquerda, aux);
    }
}

void Rot_direita_S(Node **raiz, Node *no, bool dupla = false){

    Node *filho = no->esquerda;
    if(filho == NULL){

        printf("falha na rotação\n");
        return;
    }

    Node *filho_direita = filho->direita;
    no->esquerda = filho_direita;
    filho->direita = no;

    Node *pai = busca_pai(*raiz, no->valor);

    if(pai == NULL){

        *raiz = filho;
    }else{
        if(dupla == true){
            pai->direita = filho;
        }else{
            pai->esquerda = filho;
        }
    }
}

void Rot_esquerda_S(Node **raiz, Node *no, bool dupla = false){
    Node *filho = no->direita;
    if(filho == NULL){
        printf("falha na rotação\n");
        return;
    }

    Node *filho_esquerda = filho->esquerda;

    no->direita = filho_esquerda;
    filho->esquerda = no;

    Node *pai = busca_pai(*raiz, no->valor);

    if(pai == NULL){
        *raiz = filho;
    }else{
        if(dupla == true){
            pai->esquerda = filho;
        }else{
            pai->direita = filho;
        }
    }
}

Node *root_aux(Node *no){
    Node *aux = no;
    while(aux->pai != NULL){
        aux = aux->pai;
    }
    return aux;
}

void rotDdireita(Node **raiz, Node *no){
    Node *filho = no->esquerda;
    if(filho == NULL){
        printf("falha na rotação\n");
        return;
    }
    Rot_esquerda_S(raiz, filho,true);
    Rot_direita_S(raiz, no);
}

void rotDesquerda(Node **raiz, Node *no){
    Node *filho = no->direita;
    if(filho == NULL){
        printf("falha na rotação\n");
        return;
    }
    Rot_direita_S(raiz, filho,true);
    Rot_esquerda_S(raiz, no);
}

Node *novo_no(int aux = -1){

    Node *no = (Node*) malloc(sizeof(Node));

    if(aux==-1){

        no->valor = rand()%50;
        printf("%d, ", no->valor);
    }else{

        no->valor = aux;
    }
    no->direita = NULL;
    no->esquerda = NULL;

    return no;
}

Node *busca_tio(Node *no){
    if(no->pai == NULL){
        return NULL;
    }

    Node *pai = no->pai;

    if(pai->direita == no){

        return pai->esquerda;
    }else{

        return pai->direita;
    }
}

void inserir_no( Node *no, Node *novo){

    if(no == NULL){
        printf("falha\n");
        return;
    }
    if(no->valor > novo->valor){
        printf("numero ja inserido\n");
        return;
    }

    if(no->valor < novo->valor){
        if(no->direita == NULL){
            no->direita = novo;
            
            if(no->cor == vermelho){
                no->cor = preto;
            }
            novo->pai = no;
            if((no->cor == vermelho) and (busca_tio(no)->cor == vermelho)){
                no->cor = preto;
                busca_tio(no)->cor = preto;

                if(no->pai->pai == NULL){
                    return;
                }else{
                    no->pai->cor = vermelho;
                }
            } else if((no->cor == vermelho) and (busca_tio(no)->cor == preto)){

                if((no->pai->direita == no)){

                    Node *raiz = root_aux(no);
                    Rot_esquerda_S(&raiz, no);
                    no->cor = preto;
                    no->pai->cor = vermelho;

                }else if((no->pai->esquerda == no)){

                    Node *raiz = root_aux(no);
                    Rot_esquerda_S(&raiz, novo);
                    Rot_direita_S(&raiz, novo);
                    novo->cor = preto;
                    novo->pai->cor = vermelho;
                }
            } return;
        } inserir_no(no->direita,novo);
    }else{
        if(no->esquerda == NULL){
            no->esquerda = novo;
            if(no->cor == vermelho){
                novo->cor = preto;
            }
            novo->pai = no;
            if((no->cor == vermelho) and (busca_tio(no)->cor ==  vermelho)){

                no->cor = preto;
                busca_tio(no)->cor = preto;
                if(no->pai->pai == NULL){
                    return;
            }else{
                no->pai->cor = vermelho;
            }
        }
        else if ((no->cor == vermelho) and (busca_tio(no)->cor == preto)){
            if(no->pai->esquerda = no){

                Node *raiz = root_aux(no);
                Rot_direita_S(&raiz, no);
                no->cor = preto;
                no->pai->cor = vermelho;

            }else if((no->pai->direita == no)){

                Node *raiz = root_aux(no);
                Rot_direita_S(&raiz, novo);
                Rot_esquerda_S(&raiz, novo);
                novo->cor = preto;
                novo->pai->cor = vermelho;
             }
            } return;
        }
        inserir_no(novo->esquerda, novo);
    }
}

Node *buscar_no(Node *node, int num){
    if (node == NULL){

        return NULL;
    }
    if (node->valor == num){

        return node;
    }

    if (node->valor < num){

        return buscar_no(node->direita, num);
    }
    else{

        return buscar_no(node->esquerda, num);
    }
}

void imprimir(Node *node, int tab = 0)
{
    if (node == NULL)
        return;

    imprimir(node->direita, tab + 1);
    for (int i = 0; i < tab; i++)
    {
        printf("\t");
    }
    printf("%d - ", node->valor);
    printf("%d\n", node->cor);
    imprimir(node->esquerda, tab + 1);
}

void inserir_raiz(Node **raiz){

    if (*raiz == NULL){

        *raiz = novo_no();
        (*raiz)->cor = preto;
        (*raiz)->pai = NULL;
        return;
    }
    Node *novo = novo_no();
    inserir_no(*raiz, novo);
}

void buscar_raiz(Node *raiz){

    int num = 0;
    printf("Digite um numero: ");
    scanf("%d", &num);
    Node *res = buscar_no(raiz, num);
    if (res == NULL){

        printf("Numero não encontrado");
    }
    else{

        imprimir(res);
    }
}

bool eh_folha(Node *node){

    if ((node->esquerda == NULL) && (node->direita == NULL)){
        return true;
    }
    else{

        return false;
    }
}

bool um_filho(Node *node){

    if ((node->esquerda == NULL) && (node->direita != NULL)){

        return true;
    }

    if ((node->esquerda != NULL) && (node->direita == NULL)){

        return true;
    }
    return false;
}

Node *maior(Node *node)
{
    if (node == NULL){
        return NULL;
        }
    if (node->direita == NULL){
        return node;
        }

    return maior(node->direita);
}

Node *menor(Node *node)
{
    if (node == NULL){
        return NULL;
        }
    if (node->esquerda == NULL){
        return node;
        }
    return menor(node->esquerda);
}

int soma(Node *node)
{
    if (node == NULL) return 0;

    return node->valor + soma(node->direita) + soma(node->esquerda);
}

int altura(Node *node)
{
    if (node == NULL) return 0;

    if (node->cor == vermelho){

        return 0 + fmax(altura(node->esquerda), altura(node->direita));
    }
    else{
        return 1 + fmax(altura(node->esquerda), altura(node->direita));
        }
}

int cont(Node *node){

        if (node == NULL) return 0;

        return 1 + cont(node->direita) + cont(node->esquerda);
}

float media(Node *node){ 

        float total = soma(node);
        float qnt = cont(node);
        return total / qnt;
}

int check_balance(Node *raiz)
{
        int altura_dir = altura(raiz->direita);
        int altura_esq = altura(raiz->esquerda);

        if (altura_dir - altura_esq == 0){

        return 0;

        }
        else if (altura_dir > altura_esq){

        return 1;

        }
        else return -1;
}

Node *remover(Node *node, int num);

void substituir(Node *pai, Node *removido)
{
        if (um_filho(removido))
        {
        if (removido->esquerda != NULL)
        {
            pai->esquerda = removido->esquerda;
        }
        else
        {
            pai->direita = removido->direita;
        }
        }
        else
        {
        Node *substituto = maior(removido->esquerda);
        remover(pai, substituto->valor);
        substituto->esquerda = removido->esquerda;
        substituto->direita = removido->direita;
        if (pai->valor < substituto->valor)
        {
            pai->direita = substituto;
        }
        else
        {
            pai->esquerda = substituto;
        }
        removido->direita = NULL;
        removido->esquerda = NULL;
        }
}

Node *remover(Node *node, int num){

        Node *pai = busca_pai(node, num);
        if (pai == NULL)
        {
        printf("Numero não encontrado");
        return NULL;
        }
        else
        {
        Node *removido = NULL;
        if (pai->valor < num)
        {
            removido = pai->direita;
            if (eh_folha(removido))
            {
             pai->direita = NULL;
            }
            else
            {
             substituir(pai, removido);
            }
        }
        else{

            removido = pai->esquerda;
            if (eh_folha(removido))
            {
             pai->esquerda = NULL;
            }
            else
            {
             substituir(pai, removido);
            }
        }
        return removido;
        }
}

void remover_raiz(Node **raiz){

        int num = 0;
        printf("Digite um numero: ");
        scanf("%d", &num);

        if (*raiz == NULL)
        {
        printf("Árvore vazia\n");
        return;
        }

        if ((*raiz)->valor == num){

            if (eh_folha(*raiz)){
                free(*raiz);
                *raiz = NULL;
                return;
            }

            if (um_filho(*raiz)){

                Node *remover = *raiz;
                if ((*raiz)->esquerda != NULL){

                *raiz = (*raiz)->esquerda;
                }
                else{
                *raiz = (*raiz)->direita;
                }
                free(remover);
                }
            else{

            Node *substituto = maior((*raiz)->esquerda);
            remover(*raiz, substituto->valor);
            substituto->esquerda = (*raiz)->esquerda;
            substituto->direita = (*raiz)->direita;
            (*raiz)->esquerda = NULL;
            (*raiz)->direita = NULL;
            free(*raiz);
            *raiz = substituto;
             }
        }else{
            remover(*raiz, num);
        }
}

void fb_raiz(Node *raiz){

        int num = 0;
        printf("Digite o numero: ");
        scanf("%d", &num);
        Node *res = buscar_no(raiz, num);

        if (res == NULL){

            printf("Numero não encontrado");
        }
        else{

            printf("FB do nó:%d\n", altura(res->esquerda) - altura(res->direita));
        }
}

int fb_no(Node *no){

        if (no == NULL)
            return 0;
        return altura(no->esquerda) - altura(no->direita);
}

void imprimir_fb(Node *node, int tab = 0){

        if (node == NULL)
            return;

        imprimir_fb(node->direita, tab + 1);
        for (int i = 0; i < tab; i++)
        {
            printf("\t");
        }
        printf("%d\n", altura(node->esquerda) - altura(node->direita));
        imprimir_fb(node->esquerda, tab + 1);
}

void altura_no(Node *raiz){

        int num = 0;
        printf("Digite um numero: ");
        scanf("%d", &num);
        Node *res = buscar_no(raiz, num);
        if (res == NULL)
        {
            printf("Numero não encontrado");
        }
        else
        {
            printf("Altura nó:%d\n", altura(res));
        }
}

void arvore_aleatoria(Node **root)
{
        for (int i = 0; i < 10; i++){

            inserir_raiz(root);
        }
}

void balancear(Node **raiz, Node *no)
{
        if (no == NULL)
            return;

        if (fb_no(no) <= -2)
        { // Rotacão esquerda
            if (fb_no(no->direita) < 1)
            { // Rotação simples
            Rot_esquerda_S(raiz, no);
            }
            else
            { // Rotação dupla
            rotDesquerda(raiz, no);
            }
        }

        if (fb_no(no) >= 2)
        { // Rotacão direita

            if (fb_no(no->esquerda) > -1)
            { // Rotação simples
            Rot_direita_S(raiz, no);
            }
            else
            { // Rotação dupla
            rotDdireita(raiz, no);
            }
        }
}

void menu(){

        printf("|1-Inserir                |\n");
        printf("|2-Buscar                 |\n");
        printf("|3-Imprimir Arvore        |\n");
        printf("|4-Remover                |\n");
        printf("|5-Arvore aleatoria       |\n");
        printf("|6-Verificar balanceamento|\n");
        printf("|7-Altura arvore          |\n");
        printf("|8-Altura no              |\n");
        printf("|9-FB do no               |\n");
        printf("|10-Imprimir FB           |\n");
        printf("|0-Sair                   |\n");
}

int main()
{
        srand(time(NULL));

        node *root = NULL;

        int opc = 1;
        while (opc != 0)
        {
            menu();
            scanf("%d", &opc);
            switch (opc){
                case 1:
                    inserir_raiz(&root);
                    break;
                case 2:
                    buscar_raiz(root);
                    break;
                case 3:
                    imprimir(root);
                    break;
                case 4:
                    remover_raiz(&root);
                    break;
                case 5:
                    arvore_aleatoria(&root);
                    break;
                case 6:
                    if (check_balance(root) == 0){

                    printf("A arvore esta balanceada\n");
                }else{

                    if (check_balance(root) == 1){

                        printf("A arvore esta desbalanceada a direita\n");
                }else{
                    printf("A arvore esta desbalanceada a esquerda\n");
                }
                    }
                    break;

                case 7:

                    printf("Altura: %d\n", altura(root));

                    break;

                 case 8:
                    altura_no(root);
                    break;

                case 9:
                    fb_raiz(root);
                    break;

                case 10:
                    imprimir_fb(root);
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