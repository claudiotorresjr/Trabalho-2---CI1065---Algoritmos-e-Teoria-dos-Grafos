
/*
Possui a declarações usadas
para manipular o TAD fila de prioridades,
criando um sistema de 'atender clientes'.
Os protótipos de função e estruturas
estão em fila_p.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "filap.h"

/* Cria uma lista vazia */
int cria_filap(filap* p){

    int i;

    p->inicio = NULL;
    p->fim = NULL;        
    p->tamanho = 0;

    for (i = 0; i <= 5; i++){
        p->fimpriors[i] = 0;
    }

    return 1;
}

/* Retorna 1 se a lista está vazia e zero caso contrário; */
int filap_vazia(filap* p){

    if ((p->tamanho == NULL))
        return 1;
    return 0;
}

/* Cria uma prioridade aleatória e chama
   a inserção desse cliente */
void gera_cliente(filap* p){

    int posicao;
    int x;
 
    x = rand() % (5) + 1;    

    if (filap_vazia(p)){       
        insere_inicio_filap(x, p);        
        return;
    }
    
    /* encontra a posicao para inserior */
    posicao = (p->fimpriors[x]) + 1;    

    if (posicao == 1){
        insere_inicio_filap(x, p);        
        return;
    }

    insere_posicao(x, posicao, p);

}

/* Insere x no inı́cio da fila de prioridades;  */
void insere_inicio_filap(int x, filap* p){
    
    struct cliente * novo;  
    int i;

    /* faz malloc e testa */
    novo = malloc(sizeof(struct cliente));
    if (novo == NULL){
        printf("Erro na alocação da memória");
        exit(0);
    }
    
    /* adiciona o elemento */
    novo->prior = x;    
    novo->prox = p->inicio;

    if (!filap_vazia(p))
        p->inicio->ant = novo;

    if (filap_vazia(p))
        p->fim = novo;

    novo->ant = NULL;
    p->inicio = novo;
    
    /* ajusta o vetor do fim das prioridades */
    for (i = 1; i <= x; i++){
        p->fimpriors[i]++;
    }

    p->tamanho++;   
    
}

/* Insere x na posicao pos da lista de prioridades*/ 
void insere_posicao(int x, int pos, filap* p){

    struct cliente * novo;
    struct cliente * aux;   
    int i;     

    /* faz malloc e o testa */
    novo = malloc(sizeof(struct cliente));
    if (novo == NULL){
        printf("Erro na alocação da memória");
        exit(0);
    }   

    novo->prior = x;

    /* se for para inserir no final */
    if (pos == p->tamanho+1){
        /* adiciona o elemento */      
        p->fim->prox = novo;  
        novo->ant = p->fim;
        novo->prox = NULL;   
        p->fim = novo;        
    }
    else{
        /* vai até a posicao */
        aux = p->inicio;
        for (i = 1; i < pos; i++){
            aux = aux->prox;
        }

        /* adiciona o elemento */
        novo->prox = aux;
        novo->ant = aux->ant;
        aux->ant->prox = novo;   
        aux->ant = novo;   
    }
    
    /* ajusta o vetor do fim das prioridades */
    for (i = 1; i <= x; i++){
        p->fimpriors[i]++;
    } 

    p->tamanho++;  
}

/* Remove o 1o elemento da fila
   Retorna a prioridade desse elemento*/
int remove_primeiro_filap(struct filap* p){    

    struct cliente* aux;
    int prioridade;
    int i;
  
    if (filap_vazia(p))
        return 0;    
 
    aux = p->inicio;

    prioridade = aux->prior;   /* guarda a prioridade para o sleep */  

    p->inicio = aux->prox;     /* tira o primeiro dado */
    p->inicio->ant = NULL;

    free(aux); 

    /* ajusta o vetor do fim das prioridades */
    for (i = 1; i <= prioridade; i++){
        p->fimpriors[i]--;
    }  
    
    p->tamanho--; 

    return prioridade;     
}  

/* Remove todos os elementos da fila e aponta ela para NULL */
void destroi_filap(filap* p){
    struct cliente* aux;
 
    if(filap_vazia(p)){
        printf("não destruida: lista vazia\n");
    }

    while (p->inicio != NULL){
        aux = p->inicio;
        p->inicio = aux->prox;
        p->tamanho--;
        free(aux);
    }        
}

/* Processa os clientes, fica indisponível enquanto 'atende'
   adiciona um cliente cada vez que um for processado, 
   até o tempo de atendimento chegar em ou passar de 30 */
int processa_filap(filap* p){
    
    int prioridade;
    int atendidos[6] = {0,0,0,0,0,0};
    int tempo_total;
    int i;
   
    printf("+++ FIM DO EXPEDIENTE +++\n");
    printf("ATENDIDOS:\n");
    
    tempo_total = 0;    
    
    /* remove o primeiro elemento, aumenta o tempo e gera um novo cliente */
    while (tempo_total <= 30){        
        prioridade = remove_primeiro_filap(p); 
        tempo_total = tempo_total + prioridade;  
        sleep(prioridade);
        atendidos[prioridade]++;
        gera_cliente(p);
    }

    for (i = 1; i <= 5; i++){
        printf("Prioridade %d: %d\n", i, atendidos[i]);
    }

    printf("Total do expediente: %d\n", tempo_total);

    destroi_filap(p);
    
}
