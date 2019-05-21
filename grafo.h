#ifndef _GRAFO_H
#define _GRAFO_H

#include <stdio.h>

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo
// 
// o grafo tem um nome, que é uma "string"

typedef struct _grafo *grafo;

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um vértice
// 
// o vértice tem um nome, que é uma "string"

typedef struct _vertice *vertice;

//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
// 
// devolve 1 em caso de sucesso,
//         ou 
//         0, caso contrário

int destroi_grafo(grafo g);

//------------------------------------------------------------------------------
// lê um grafo de input.
// um grafo é um arquivo onde cada linha tem zero, uma ou duas
// strings (sequência de caracteres sem "whitespace").
// 
// devolve o grafo lido. Caso o arquivo esteja mal formado o
// comportamento da função é indefinido

grafo le_grafo(FILE *input);  

//------------------------------------------------------------------------------
// escreve o grafo g em output, no mesmo formato que o usado por le_grafo()
//
// devolve o grafo escrito,
//         ou 
//         NULL, em caso de erro 

grafo escreve_grafo(FILE *output, grafo g);

//------------------------------------------------------------------------------
// devolve o número de vértices de g

unsigned int n_vertices(grafo g);

//------------------------------------------------------------------------------
// devolve o número de arestas de g

unsigned int n_arestas(grafo g);

//------------------------------------------------------------------------------
// devolve o vértice de nome 'nome' em g
//         ou
//         NULL, caso não exista tal vertice

vertice vertice_nome(grafo g, char *nome);

//------------------------------------------------------------------------------
// devolve o nome de v

char *nome(vertice v);

//------------------------------------------------------------------------------
// devolve o número mínimo k de trilhas necessárias para cobrir o grafo g
//
// observe que 
//
//   k = 1, se g é euleriano, ou
//   k = (número de vértices de grau ímpar em g)/2, caso contrário
//
// aloca e preenche o vetor cobertura de maneira que cobertura[i] contém uma 
// trilha em G para cada 0 <= i < k.
//
// cada trilha é um sequência de vertices terminada por NULL, isto é, para cada 0 <= i < k
// a sequência 
//
//     (cobertura[i][0], cobertura[i][1], ..., cobertura[i][l-1], cobertura[i][l])
//
// é tal que
//
//     1. cobertura[i][j] é um vertice, para cada 0 <= j < l,
//     2. cobertura[i][l] == NULL, e
//     3. cobertura[i][j-1] é vizinho de cobertura[i][j] em g, para cada 0 < j < l

unsigned int cobertura_por_trilhas(grafo g, vertice **cobertura[]);

//------------------------------------------------------------------------------
#endif
