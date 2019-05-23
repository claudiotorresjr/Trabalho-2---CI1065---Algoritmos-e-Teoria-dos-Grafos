#include <stdio.h>
#include "grafo.h"
#include <stdint.h>
#include <stdlib.h>

int main(void) {
  grafo g = le_grafo(stdin);
  if (!g) { return 1; }

  vertice** cobertura;
  unsigned int quantidade = cobertura_por_trilhas(g, &cobertura);
  printf("# %d trilhas\n", quantidade);

  for (unsigned int i = 0; i < quantidade; i++) {
      fprintf(stderr, "Trilha %d: ", i + 1);
      int j = 0;
      while (cobertura[i][j] != NULL) {
          fprintf(stderr, "%s ", nome(cobertura[i][j]));
          j++;
      }

      free(cobertura[i]);
      fprintf(stderr, "\n");
  }

  free(cobertura);

  printf("# %d arestas\n", n_arestas(g));
  printf("# %d vertices\n", n_vertices(g));
  escreve_grafo(stdout, g);

  return !destroi_grafo(g);
}
