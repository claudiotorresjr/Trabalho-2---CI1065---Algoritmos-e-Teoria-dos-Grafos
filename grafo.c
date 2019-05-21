#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grafo.h"
/*------------------------------------------------------------------------------
        Struct para a lista de vertices de grau impar
    --> struct vertice *head; ponteiro para o inicio da lista
    --> struct vertice *tail; ponteiro para o fim da lista
------------------------------------------------------------------------------*/
struct listaVertices
{
	struct _vertice *head;
	unsigned int tam;

}__attribute__((packed));
typedef struct listaVertices *listaVertices;

/*------------------------------------------------------------------------------
        Struct para a lista de adjacencia
    --> struct vertice *head; ponteiro para o inicio da lista
    --> struct vertice *tail; ponteiro para o fim da lista
------------------------------------------------------------------------------*/
struct AdjList
{
	struct _vertice *head;
	//struct _vertice *tail;

};
typedef struct AdjList *AdjList;

/*------------------------------------------------------------------------------
        Struct para o grafo
    --> unsigned int numV; numero de vertices
	--> unsigned int numA; numero de arestas
	--> struct AdjList *array; um array de listas de adjacencias (cada indice uma lista)
------------------------------------------------------------------------------*/
struct _grafo
{
	unsigned int numV; 
	unsigned int numA;
	unsigned int achouCiclo;
	struct _vertice *raizArvore;
	struct AdjList *array;
}__attribute__((packed));

/*------------------------------------------------------------------------------
        Struct para o vertice
    --> int estado; 
       +0 nao foi visitado, (branco)
       +1 foi visitado, (vermelho)
       +2 faz parte de um ciclo, (verde)
       +-1 vertice v adicionado a G 
    --> unsigned int grau; grau do vertice
	--> char *nome; nome do vertice
	--> int coringa: se 1, indica que o vertice nao faz parte do arquivo do grafo
	--> struct vertice *next; ponteiro para o proximo vertice
	--> struct vertice *pai; ponteiro para o pai do vertice
------------------------------------------------------------------------------*/
struct _vertice
{	
	int estado;
	int pre;
	int pos;
	unsigned int grau;
	char *nome;
	int coringa;
	struct _vertice *next;
	struct _vertice *pai;

}__attribute__((packed));

/*------------------------------------------------------------------------------
        Coloca os pares de vertices na sua devida lista de adjacencia
    Recebe o grafo, cada linha lida do arquivo, o separador caso tenha duas strings
    e numero de espacos para que, caso tenha mais de um (mais de duas strings)
    essa linha nao seja utilizada.
------------------------------------------------------------------------------*/
void coloca_vertice_grafo(grafo g, char *line, long unsigned int espaco, unsigned int numEspaco);

/*------------------------------------------------------------------------------
        Cria um vertice (no da lista de adjacencia)
    Recebe o nome do vertice que sera alocado e o tamanho dessa String e retorna
    o novo vertice criado
------------------------------------------------------------------------------*/
vertice cria_vertice_lista(char *nome, long unsigned int tam);

/*------------------------------------------------------------------------------
        Confere se os pares de vertices ja possuem uma aresta entre eles
    Recebe o grafo e um par de vertices e retorna 1 caso ja exista a aresta
------------------------------------------------------------------------------*/
int tem_aresta(grafo g, char *verticeA, char *verticeB);

/*------------------------------------------------------------------------------
        Cria o grafo e retorna o grafo criado
------------------------------------------------------------------------------*/
grafo cria_grafo(void);

/*------------------------------------------------------------------------------
        Adiciona uma aresta entre dois vertices (adiciona o verticeB na lista de 
    adjacencia de verticeA e vice versa)
    Caso verticeB esteja nulo (espaco em branco por exemplo), coloco o verticeA
    como "cabeca" de uma lista de adjacencia. 
------------------------------------------------------------------------------*/
void add_aresta(grafo g, char *verticeA, char *verticeB);

/*------------------------------------------------------------------------------
        Inicia uma lista vazia
------------------------------------------------------------------------------*/
listaVertices inicialista(void);

/*------------------------------------------------------------------------------
        Insere um vertice vert na lista
------------------------------------------------------------------------------*/
void insereListaFinal(listaVertices lista, vertice vert);

/*------------------------------------------------------------------------------
        Retira da lista pelo final
------------------------------------------------------------------------------*/
vertice retiraFinalLista(listaVertices lista);

/*------------------------------------------------------------------------------
        Mostra toda a lista
------------------------------------------------------------------------------*/
void mostraLista(listaVertices lista);

/*------------------------------------------------------------------------------
        Verifica os vertices que se encontra no ciclo encontrado.
------------------------------------------------------------------------------*/
void verificaCicloT(grafo g, listaVertices lista, listaVertices listaTrilhaT);

/*------------------------------------------------------------------------------
       Remove a aresta v-u / u-v do grafo 
------------------------------------------------------------------------------*/
void removeAresta(grafo g, vertice v, vertice u);

/*------------------------------------------------------------------------------
        Libera todos os nodes da lista
------------------------------------------------------------------------------*/
void freeLista(listaVertices lista);

/*------------------------------------------------------------------------------
       Para cada vertice na lista do ciclo encontrado, arruma a trilha listaTrilhaT
    para comportar o ciclo menor (listaTrilhaT ainda continua como uma trilha)
------------------------------------------------------------------------------*/
void atualizaTrilha(listaVertices copia, listaVertices listaTrilhaT);

/*------------------------------------------------------------------------------
        Realiza uma busca em profundidade no grafo g, salvando a arvoreT
------------------------------------------------------------------------------*/
void buscaProfundidade(listaVertices lista, listaVertices listaTrilhaT, grafo g, vertice vert);

grafo clonaGrafo(grafo g);
vertice copiaVertice(vertice v);
int arestaProcessada(vertice vert, vertice v, listaVertices lista);
//void mostraCobertura(vertice **cobertura[], unsigned int k);

/*------------------------------------------------------------------------------
       tamanho maximo do buffer (que eh o tamanho maximo de cada linha do arquivo)
------------------------------------------------------------------------------*/
#define buffSize 2050

/*------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------*/
void coloca_vertice_grafo(grafo g, char *line, long unsigned int espaco, unsigned int numEspaco)
{
	// aloco espaco para duas Strings. Cada uma representando um vertice
	char *verticeA = (char*)calloc(buffSize/2, sizeof(char));
	char *verticeB = (char*)calloc(buffSize/2, sizeof(char));

	//se nao houver o separador de Strings (espaco no caso), deixo o nome do verticeB em branco
	//e so crio o verticeA
	if(numEspaco == 0)
	{	
		strcpy(verticeA, line);					
		strcpy(verticeB, " ");
	}
	//caso contraio, o verticeA recebe tudo antes do espaco e o verticeB recebe o restante
	//como verticeA eh a primeira metade da linha, colocamos um \0 no final 
	else
	{	
		strncpy(verticeA, line, espaco);
		strncpy(verticeB, &line[espaco + 1], strlen(line));
		verticeA[strlen(verticeA)+1] = '\0';
	}
	//printf("%s %s\n", a, b);
	//o processo de inserir vertice continua somente se os nomes dos vertice forem de ate 1024 caracteres
	if(strlen(verticeA) <= 1024 && strlen(verticeB) <= 1024)
	{
		//printf("linha: %s\n", line);
		//printf("primeira coluna: %s de tamanho: %zu segunda coluna: %s de tamanho: %zu\n\n", a, strlen(a), b, strlen(b));
		
		//adiciono os vertices no grafo			
		add_aresta(g, verticeA, verticeB);

		//caso o array de lista de adjacencias esteja todo usado (numero de vertices atingiu o limite de listas criadas)
		//o array eh realocado para que caiba mais vertices (listas)
		//do modo que foi feito, a cada numero de vertices multiplo de 100, eh criado mais 100 espacos para listas
		if((g->numV%100 == 0) && (g->numV != 0))
		{
			//printf("realoquei mais 100 espacos na lista de adjacencia\n");
			g->array = (AdjList)realloc(g->array, (g->numV+100)*sizeof(struct AdjList));
			for(unsigned int k = 1; k <= 100; ++k)
			{
				g->array[g->numV+k].head = NULL;
			}
		}
	}
	else
	{
		//printf("Tamanho da String invalido\n\n");
	}
	free(verticeA);
	free(verticeB);
}

vertice cria_vertice_lista(char *nome, long unsigned int tam)
{
	//aloco espaco para um vertice e tambem para a String que representa seu nome
	vertice novo_vertice = (vertice)malloc(sizeof(struct _vertice));
	novo_vertice->nome = (char*)malloc(tam*sizeof(char));
	strcpy(novo_vertice->nome, nome);
	novo_vertice->next = NULL;
	novo_vertice->grau = 0;
	novo_vertice->estado = 0;
	novo_vertice->coringa = 0;

	//retorna o vertice criado
	return novo_vertice;
}

int tem_aresta(grafo g, char *verticeA, char *verticeB)
{
	vertice aux;

	unsigned int i = 0;
	//printf("verificando %s com %s\n", verticeA, verticeB);
	//enquanto nao chegou ao final do array de lista de adjacencias, procuro a existencia de uma aresta
	//entre dois vertices dados
	while(g->array[i].head != NULL)
	{	
		//caso a "cabeca" de uma lista for igual ao verticeA, confiro se o verticeB esteja em sua lista
		if(strcmp(g->array[i].head->nome, verticeA) == 0)
		{
			aux = g->array[i].head->next;
			while(aux != NULL)
			{	
				//se verticeB estiver na lista de A, retorno verdadeiro, indicando que ha uma aresta entre
				//esses dois vertices
				if(strcmp(aux->nome, verticeB) == 0){
					//printf("Ja existe a aresta dos vertices %s----%s\n", verticeA, verticeB);
					return 1;
				}
				aux = aux->next;
			}
			//retorno falso caso nao tenha encontrado uma aresta
			return 0;
		}
		i++;
	}
	return 0;
}

void add_aresta(grafo g, char *verticeA, char *verticeB)
{
	vertice novo_vertice;
	
	unsigned int i = 0, achou = 0;

	//salvo o tamanho da String de cada vertice para alocar a String de tamanho correto
	long unsigned int tamA = strlen(verticeA)+1, tamB = strlen(verticeB)+1;

	//se houver uma aresta entre verticeA e verticeB, sai da funcao de adicionar aresta
	if(tem_aresta(g, verticeA, verticeB))
	{
		return;
	}
	//inserindo verticeB na lista do verticeA
	//enquanto nao foi encontrado uma posicao vazia no array de listas de adjacencia ou o elemento 
	//da "cabeca" nao for igual ao verticeA, o indice vai sendo incrementado 
	while(!achou)
	{	
		//se a "cabeca" estiver vazia, adiciono o verticeA 
		if(g->array[i].head == NULL)
		{			
			g->numV++;
			//crio um novo vertice
			novo_vertice = cria_vertice_lista(verticeA, tamA);
			g->array[i].head = novo_vertice;
			//g->array[i].tail = novo_vertice;
			g->array[i].head->grau = 0;
			achou = 1;
		}
		//se o verticeA ja estiver na cabeca, adiciono o verticeB em sua lista de adjacencia
		if(strcmp(g->array[i].head->nome, verticeA) == 0 && strcmp(verticeB, " ") != 0)
		{	
			//a insercao eh feita pela cauda
			novo_vertice = cria_vertice_lista(verticeB, tamB);	
			vertice aux = g->array[i].head;
			while(aux->next != NULL)
			{
				aux = aux->next;
			}
			aux->next = novo_vertice;		
			//g->array[i].tail->next = novo_vertice;
			//g->array[i].tail = novo_vertice;
			g->array[i].head->grau++;
			achou = 1;
		}
		i++;
	}

	//inserindo verticeA na lista do verticeB
	//(todo comentario do while anterior serve para este pois como adicionamos verticeB na lista do verticeA, precisamos
	//adicionar o verticeA na lista do verticeB)
	i = 0, achou = 0;
	while(!achou && strcmp(verticeB, " ") != 0)
	{
		if(g->array[i].head == NULL)
		{
			g->numV++;
			novo_vertice = cria_vertice_lista(verticeB, tamB);
			g->array[i].head = novo_vertice;
			//g->array[i].tail = novo_vertice;
			g->array[i].head->grau = 0;
			achou = 1;
		}
		if(strcmp(g->array[i].head->nome, verticeB) == 0)
		{	
			novo_vertice = cria_vertice_lista(verticeA, tamA);
			vertice aux = g->array[i].head;
			while(aux->next != NULL)
			{
				aux = aux->next;
			}
			aux->next = novo_vertice;			
			//g->array[i].tail->next = novo_vertice;
			//g->array[i].tail = novo_vertice;
			g->array[i].head->grau++;
			achou = 1;
		}
		i++;
	}
	//incrementamos 1 no numero de arestas do grafo
	g->numA++;
}

grafo cria_grafo(void)
{
	//aloco memoria para o grafo e inicializo suas variaveis
	grafo g = (grafo)malloc(sizeof(struct _grafo));
	g->numV = 0;
	g->numA = 0;
	g->achouCiclo = 0;

	//cria um array de listas de adjacencia
	g->array = (AdjList)malloc(100*sizeof(struct AdjList));

	//inicializa cada lista de adjacencia fazendo o no principal == NULL
	for(unsigned int i = 0; i < 100; ++i)
	{
		g->array[i].head = NULL;
		//g->array[i].tail = NULL;
	}

	return g;
}

int destroi_grafo(grafo g)
{
	//para cada vertice (indice do array de listas de adjacencia) dou free() nas Strings,
	//em cada no e ao final, no array de listas e no grafo. Retorna 1 se houver sucesso.
	for(unsigned int i = 0; i < g->numV; ++i)
	{
		vertice aux, v = g->array[i].head;

		while(v != NULL)
		{
			aux = v->next;
			free(v->nome);
			free(v);
			v = aux;
		}
	}
	free(g->array);
	free(g);

	return 1;
}

grafo le_grafo(FILE *input)
{
	//recebe o grafo criado por cria_grafo()
	grafo g = cria_grafo();


	char *line = (char*)malloc(buffSize*sizeof(char));
	int ch;
	
	unsigned int j = 0, numEspaco = 0, espaco = 0;

	//enquanto houver letras no arquivo, vou atribuindo esta letra a um vetor chamado line
	while( (ch = fgetc(input)) != EOF )
	{	
		line[j] = (char)ch;
		j++;
		
		//sempre que encontrar um espaco em branco na linha, contamos o numero de ocorrencias
		//para tratar se inserimos os vertices ou nao (numEspaco) e salvamos a posicao do primeiro
		//espaco lido (espaco)
		if(ch == ' ')
		{
			numEspaco++;
			espaco = j - 1;
		}	

		//ao encontrar um \n, chegamos ao final de uma linha 
		if(ch == '\n')
		{	
			//o buffer da linha recebe \0 no final para apagar o \n e indicar final de String
			line[j - 1] = '\0';
			//se houver mais que 1 espaco, a linha eh invalida pois possui mais de duas Strings
			if(numEspaco > 1)
			{
				//printf("LINHA %s INVALIDA pois possui mais de duas Strings\n\n", line);
			}
			//se a linha estiver vazia, nao fazemos nada
			else if(line[0] == '\0')
			{
				//printf("Linha vazia\n\n");
			}
			//se houver uma ou duas Strings, criamos um vertice para cada e adicionamos no vertice
			else
			{	
				coloca_vertice_grafo(g, line, espaco, numEspaco);
			}

			free(line);
			j = 0;
			numEspaco = 0;
			line = (char*)malloc(buffSize*sizeof(char));
			
		}

	}
	//por estar sendo usado o fgetc(), se nao houver um \n no final do arquivo, a ultima linha nao eh lida.
	//por isso essa ultima linha, se existir (nao for vazia), deve ser tratada fora do while principal. 
	//eh colocado \0 no final para indicar final de String
	line[j] = '\0';
	//se o buffer nao estiver vazio, eh feito todo o tratamento do --if(ch == '\n')-- feito anteriormente
	if(line[0] != '\0')
	{
		if(numEspaco > 1)
		{
			//printf("LINHA %s INVALIDA pois possui mais de duas Strings\n\n", line);
		}
		else if(line[0] == '\0')
		{
			//printf("Linha vazia\n\n");
		}
		else
		{	
			coloca_vertice_grafo(g, line, espaco, numEspaco);
		}
	}

	free(line);
	//fechamos o arquivo que foi lido
	fclose(input);

   
	return g;
}

grafo escreve_grafo(FILE *output, grafo g)
{
	//caso o arquivo de saida seja vazio, avisamos o usuario e retornamos o grafo
	if(output==NULL){
		printf("output is NULL\n");
		return g;
	}

	//unsigned int v;
	vertice aux;

    //printf("\nGrafo com %d vertices e %d arestas\n\n", n_vertices(g), n_arestas(g));
    //for (v = 0; v < g->numV; ++v)
    //{	
	//	printf("\n(vertice %s de grau %d)\n", g->array[v].head->nome, 
	//	g->array[v].head->grau);
	//	vertice vert = g->array[v].head->next;
	//	while(vert)
	//	{	
	//		if(vert != NULL)
	//		{
	//			printf(" -> %s(%d)", vert->nome, vert->grau);
	//			vert = vert->next;
	//		}
	//	}
	//	printf("\nhead %s\n", g->array[v].head->nome);
	//	printf("\n");
	//}

	//para cada indice do array de listas de adjacencia, percorro vertice por vertice e cada par 
	//eh colocado em um arquivo de saida no mesmo estilo do arquivo de entrada (leitura)
	//nao foi tratado a repeticao de arestas, portanto cada aresta eh mostrada duas vezes 
	//exemplo: vertice a com vertice b e vertice b com vertice a eh mostrado como (a b) e (b a) 
	//respectivamente. (saida nao possui os parenteses)
	for(unsigned int i = 0; i < g->numV; ++i)
	{	
		if(g->array[i].head != NULL)
    	{
			aux = g->array[i].head;
			while(aux->next != NULL)
			{	
				//fprintf(output, "%s %s\n", g->array[i].head->nome, aux->next->nome);
				aux = aux->next;
			}
		}
	}
	//fclose(output)

	return g;
}

unsigned int cobertura_por_trilhas(grafo g, vertice **cobertura[])
{	
	//para verificar se o grafo eh Euleriano antes de adicionar o vertice "novo"
	unsigned int ehEuleriano = 0;
    
    //copia do grafo g para nao perder o conteudo e formato original

	grafo gCopia = clonaGrafo(g);
//--------------------1.1-------------------------------
//Seja I(lista) o conjunto de vértices de grau ímpar de G.
	vertice vert;
	//crio uma lista vazia
	listaVertices lista = inicialista();
	if(!lista)
	{
		printf("Sem memoria disponivel!\n");
		exit(1);
	}

	//para cada vertice do grafo, confiro qual tem grau impar
	//se for impar, adiciono na lista
	for(unsigned int i = 0; i < gCopia->numV; ++i)
	{	
		if(gCopia->array[i].head->grau % 2 != 0){
			//crio uma copia do vertice impar para ser adicionado na lista
			vert = copiaVertice(gCopia->array[i].head);
			insereListaFinal(lista, vert);
		}
	}

//---------------------------------------------------------------------
//--------------------1.2-------------------------------
	//Se I(lista) não é vazio, acrescente um novo vértice v(novo) a G
	//e acrescente arestas de forma que v seja vizinho de todos os vértices de I. 
	//Observe que agora o grafo G é euleriano.
	if(lista->head != NULL)
	{	
		
		//mostraLista(lista);

		char novo[5] = "novo";
		vert = cria_vertice_lista(novo, 5);
		vert->estado = 0;
		vertice aux = lista->head;
		while(aux != NULL)
		{	
			//crio uma nova string com os nomes dos vertices concatenados separados por espaco em branco
			//pois a funcao coloca_vertice_grafo precisa dessa formatacao
			char *line = (char*)malloc(buffSize*sizeof(char));
			strcpy(line, vert->nome);
			strcat(line, " ");
			strcat(line, aux->nome);
			coloca_vertice_grafo(gCopia, line, strlen(vert->nome), 1);
			aux = aux->next;
			free(line);
		}
		free(vert);

		gCopia->array[n_vertices(gCopia) - 1].head->coringa = 1;

		ehEuleriano = (lista->tam)/2;
		printf("Grafo Nao Euleriano\n");
	}
	else
	{
		printf("Grafo Euleriano\n");
		ehEuleriano = 1;
	}
	freeLista(lista);

	//printf("***************************\n");
	//gCopia = escreve_grafo(stdout, gCopia);
	//printf("***************************\n");
//---------------------------------------------------------------------
//--------------------1.3-------------------------------
    //Encontre uma trilha euleriana fechada T em G.
    //--------------------2.1-------------------------------
    //Seja v um vértice de G e seja T a trilha T=(v).

    //--------------------2.2-------------------------------
    //Enquanto existir em V(T) um vértice u de grau positivo em G,
    vertice w, x, u;

	listaVertices listaTrilhaT;
	listaVertices listaBusca;

	listaTrilhaT = inicialista();
	listaBusca = inicialista();

	if(ehEuleriano != 1)
	{
		u = gCopia->array[n_vertices(gCopia) - 1].head;
	}
	else
	{
		u = gCopia->array[0].head;
	}
	gCopia->raizArvore = u;
	buscaProfundidade(listaBusca, listaTrilhaT, gCopia, u);
	freeLista(listaBusca);

	u = listaTrilhaT->head;
	while(u != NULL)
	{	
		listaBusca = inicialista();
		gCopia->achouCiclo = 0;
		w = listaTrilhaT->head;
		//atualizando grau na trilha
		while(w != NULL)
		{	
			for(unsigned int i = 0; i < n_vertices(gCopia); ++i)
			{
				if((strcmp(w->nome, gCopia->array[i].head->nome) == 0))
				{
					w->grau = gCopia->array[i].head->grau;
					w->coringa = gCopia->array[i].head->coringa;
					break;
				}
			}
			w = w->next;
		} 	
		free(w);		
	
		if(u->grau > 0)
		{
			//zero todos os estados de todos os vertices
			int achou = 0;
			for(unsigned int i = 0; i < n_vertices(gCopia); ++i)
			{
				if(!achou && (strcmp(gCopia->array[i].head->nome, u->nome) == 0))
				{
					x = gCopia->array[i].head;
					achou = 1;
				}
				w = gCopia->array[i].head;
				while(w != NULL)
				{
					w->estado = 0;
					w = w->next;
				}
			}
			free(w);
			//encontre um ciclo C em G contendo o vértice x,
			gCopia->raizArvore = x;
			buscaProfundidade(listaBusca, listaTrilhaT, gCopia, x);
		}	
		else
		{
			u = u->next;
		}
		freeLista(listaBusca);
	}
	free(u);

	mostraLista(listaTrilhaT);
	destroi_grafo(gCopia);
	
	unsigned int i, j;

	*cobertura = malloc(ehEuleriano*sizeof(vertice*));
	for(i = 0; i < ehEuleriano; ++i)
	{
		(*cobertura)[i] = malloc((listaTrilhaT->tam+1)*sizeof(struct _vertice*));
	}

	if(ehEuleriano != 1)
	{
		vert = listaTrilhaT->head->next;
	}
	else
	{
		vert = listaTrilhaT->head;
	}
	for(i = 0; i < ehEuleriano; ++i)
	{	
		for(j = 0; (vert->coringa != 1) && (vert->next != NULL); ++j)
		{	
			(*cobertura)[i][j] = copiaVertice(vert);
			vert = vert->next;
		}
		if(ehEuleriano != 1)
		{
			vert = vert->next;
			(*cobertura)[i][j] = NULL;
		}
		else
		{
			(*cobertura)[i][j] = copiaVertice(vert);
			(*cobertura)[i][j+1] = NULL;
		}
	}
	freeLista(listaTrilhaT);

	return (ehEuleriano);
}	

void buscaProfundidade(listaVertices lista, listaVertices listaTrilhaT, grafo g, vertice vert)
{	
	vertice u, aux1, aux2;

	vert->estado = 1;

	for (unsigned int i = 0; i < n_vertices(g); ++i)
    {
		u = g->array[i].head;
		while(u != NULL)
		{
			if((strcmp(u->nome, vert->nome) == 0))
			{
				u->estado = 1;
			}
			u = u->next;
		}
	}

	vertice v = vert->next;
	
	while(v != NULL)
	{	
		//printf("2passei de %s -> %s\n", vert->nome, v->nome);
		if(v->estado == 1 && !arestaProcessada(vert, v, lista) && strcmp(g->raizArvore->nome, v->nome) != 0)
		{
			//printf("mudando estado\n");
			//mostraLista(lista);
			v->estado = 0;
		}
		//printf("2passei de %s -> %s\n", vert->nome, v->nome);
		if(v->estado == 1 && (strcmp(v->nome, vert->pai->nome)) && !arestaProcessada(vert, v, lista))
		{	
			if(strcmp(g->raizArvore->nome, v->nome) == 0)
			{
				aux1 = copiaVertice(vert);		
				aux2 = copiaVertice(v);
				insereListaFinal(lista, aux1);
				insereListaFinal(lista, aux2);

				//printf("1passei de %s -> %s\n", vert->nome, v->nome);
				//printf("ACHEI CICLO!!\n");
				g->achouCiclo = 1;
				verificaCicloT(g, lista, listaTrilhaT);
			}
			
		}
		else if((v->estado == 0))
		{	
			v->pai = vert;
			aux1 = copiaVertice(vert);
			aux2 = copiaVertice(v);
			insereListaFinal(lista, aux1);
			insereListaFinal(lista, aux2);

			for (unsigned int i = 0; i < n_vertices(g); ++i)
    		{
				u = g->array[i].head;
				while(u != NULL)
				{
					if((strcmp(u->nome, v->nome) == 0))
					{
						u->pai = vert;
					}
					u = u->next;
				}
			}
			//printf("2passei de %s -> %s\n", vert->nome, v->nome);
			for(unsigned int i = 0; i < n_vertices(g); ++i)
			{	
				if((strcmp(g->array[i].head->nome, v->nome) == 0))
				{
					u = g->array[i].head;
					break;
				}
			}
			buscaProfundidade(lista, listaTrilhaT, g, u);
		}
		if(g->achouCiclo == 0)
		{
				v = v->next;
		}
		else
		{	
			return;
		}
	}
	vert->estado = 2;
	for (unsigned int i = 0; i < n_vertices(g); ++i)
    {
		u = g->array[i].head;
		while(u != NULL)
		{
			if((strcmp(u->nome, vert->nome) == 0))
			{
				u->estado = 2;
			}
			u = u->next;
		}
	}

}

int arestaProcessada(vertice vert, vertice v, listaVertices lista)
{	
	vertice aux = lista->head;
	for(unsigned int i = lista->tam; i > 0; i-=2)
	{	
		if(strcmp(aux->nome, vert->nome) == 0 && strcmp(aux->next->nome, v->nome) == 0)
		{
			return 1;
		}
		else if (strcmp(aux->nome, v->nome) == 0 && strcmp(aux->next->nome, vert->nome) == 0)
		{
			return 1;
		}
		aux = aux->next->next;
	}
	return 0;
}


listaVertices inicialista(void)
{
	//aloco memoria para a lista 
	listaVertices fv = (listaVertices)malloc(sizeof(struct listaVertices));

	fv->head = NULL;
	fv->tam = 0;

	return fv;
}

void insereListaFinal(listaVertices lista, vertice vert)
{

	if(lista->head == NULL)
	{
		lista->head = vert;
	}
	else
	{
		vertice aux = lista->head;

		while(aux->next != NULL)
		{
			aux = aux->next;
		}

		aux->next = vert;
	}
	lista->tam++;
}

vertice retiraFinalLista(listaVertices lista)
{	
	vertice aux = lista->head->next;
	vertice temp = lista->head;
	vertice copia;
	if(aux != NULL)
	{
		while(aux->next != NULL)
		{	
			temp = aux;
			aux = aux->next;
		}

		temp->next = NULL;
		lista->tam--;
		copia = copiaVertice(aux);
		free(aux->nome);
		free(aux);
		return copia;
	}
	lista->tam--;
	lista->head = NULL;
	copia = copiaVertice(temp);
	free(temp->nome);
	free(temp);
	return copia;
}

void mostraLista(listaVertices lista)
{	
	if(lista->head != NULL)
	{
		printf("---- lista ----\n");
		vertice aux = lista->head;
		while(aux->next != NULL)
		{
			printf("%s --> ", aux->nome);
			aux = aux->next;
		}
		printf("%s\n", aux->nome);
	}	
}

void freeLista(listaVertices lista)
{
	if(lista->head == NULL)
	{
		return;
	}

	vertice aux = lista->head;
	lista->head = lista->head->next;	
	while(lista->head != NULL)
	{	
		free(aux->nome);
		free(aux);
		aux = lista->head;
		lista->head = lista->head->next;
	}	
	free(aux->nome);
	free(aux);	
}



void verificaCicloT(grafo g, listaVertices lista, listaVertices listaTrilhaT)
{	
	vertice v, u;

	listaVertices copia = inicialista();


	//removendo arestas do ciclo
	for(unsigned int i = lista->tam; i > 0; i-=2)
	{	
		v = retiraFinalLista(lista);
		insereListaFinal(copia, v);

		u = retiraFinalLista(lista);
		if(i == 2)
		{
			insereListaFinal(copia, u);
		}

		removeAresta(g, v, u);
	}
	atualizaTrilha(copia, listaTrilhaT);
	freeLista(copia);
}


void atualizaTrilha(listaVertices copia, listaVertices listaTrilhaT)
{
	vertice v, aux, temp;
	if(listaTrilhaT->head == NULL)
	{
		for(unsigned int i = copia->tam; i > 0; --i)
		{
			v = retiraFinalLista(copia);
			insereListaFinal(listaTrilhaT, v);	
		}
		freeLista(copia);
	}
	else
	{
		aux = listaTrilhaT->head;
		v = retiraFinalLista(copia);
		while(aux->next != NULL)
		{	
			if(strcmp(aux->nome, v->nome) == 0)
			{
				break;
			}
			aux = aux->next;
		}
		for(unsigned int i = copia->tam; i > 0; --i)
		{
			v = retiraFinalLista(copia);
			temp = aux->next;
			aux->next = v;
			v->next = temp;
			listaTrilhaT->tam++;
			aux = aux->next;
		}
		freeLista(copia);
	}
}

void removeAresta(grafo g, vertice v, vertice u)
{	
	vertice vert, aux;
	int removeu = 0;
	for(unsigned i = 0; i < n_vertices(g); ++i)
	{	
		vert = g->array[i].head;
		if(vert)
		{
			if((strcmp(vert->nome, v->nome) == 0))
			{
				while(vert->next != NULL)
				{	
					if((strcmp(vert->next->nome, u->nome) == 0))
					{
						aux = vert->next;
						vert->next = aux->next;
						free(aux->nome);					
						free(aux);

						g->array[i].head->grau--;
						removeu = 1;
						break;
					}
					vert = vert->next;
				}
			}
		}
		if(removeu)
		{
			break;
		}
	}
	removeu = 0;
	for(unsigned i = 0; i < n_vertices(g); ++i)
	{	
		vert = g->array[i].head;
		if(vert)
		{
			if((strcmp(vert->nome, u->nome) == 0))
			{	
				while(vert->next != NULL)
				{	
					if((strcmp(vert->next->nome, v->nome) == 0))
					{
						aux = vert->next;
						vert->next = aux->next;
						free(aux->nome);
						free(aux);

						g->array[i].head->grau--;
						removeu = 1;
						break;
					}
					vert = vert->next;
				}
			}
		}
		if(removeu)
		{
			break;
		}
	}
	g->numA--;
	return;
}

grafo clonaGrafo(grafo g)
{
	grafo gClone = cria_grafo();

	gClone->numV = n_vertices(g);
	gClone->numA = n_arestas(g);

	vertice u, clone;
	for (unsigned int i = 0; i < n_vertices(g); ++i)
    {
		u = g->array[i].head;
		gClone->array[i].head = copiaVertice(g->array[i].head);
		
		clone = gClone->array[i].head;
		while(u != NULL)
		{
			clone->next = copiaVertice(u->next);
			u = u->next;
			clone = clone->next;
		}
	}
	return gClone;
}

vertice copiaVertice(vertice v)
{
	if(v != NULL)
	{
		vertice copia = cria_vertice_lista(v->nome, strlen(v->nome)+1);
		copia->estado = v->estado;
		copia->pre = v->pre;
		copia->pos = v->pos;
		copia->grau = v->grau;
		strcpy(copia->nome, v->nome);
		copia->coringa = v->coringa;
		
		return copia;
	}
	return NULL;
}

void mostraCobertura(vertice *cobertura[], unsigned int k)
{	
	printf("\n------ Vetor Cobertura ------\n");
	for(unsigned int i = 0; i < k; ++i)
	{
		for(unsigned int j = 0; cobertura[i][j] != NULL; ++j)
		{	
			printf("%s --> ", cobertura[i][j]->nome);
		}
		printf("NULL\n");
	}
}

unsigned int n_vertices(grafo g)
{
	return g->numV;
}

unsigned int n_arestas(grafo g)
{
	return g->numA;
}
