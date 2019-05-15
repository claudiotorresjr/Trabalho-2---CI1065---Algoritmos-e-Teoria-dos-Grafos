#include <stdio.h>
#include "grafo.h"

//------------------------------------------------------------------------------

int main(void) {

  grafo g = le_grafo(stdin);

  if ( !g )

    return 1;
	
	vertice **cobertura = NULL;

	g = escreve_grafo(stdout, g);
	unsigned int k;
	k = cobertura_por_trilhas(g, cobertura);
    printf ("K == %u\n", k);

    return ! destroi_grafo(g);
}
