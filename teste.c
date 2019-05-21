#include <stdio.h>
#include "grafo.h"

//------------------------------------------------------------------------------

int main(void) {

  grafo g = le_grafo(stdin);

  if ( !g )

    return 1;
	
	vertice **cobertura;

	g = escreve_grafo(stdout, g);
	unsigned int k;
	k = cobertura_por_trilhas(g, &cobertura);
    printf ("K == %u\n", k);

    mostraCobertura(cobertura, k);

    return ! destroi_grafo(g);
}
