#include <stdio.h>

typedef unsigned int uint;

typedef struct Conjunto {
        uint tam;
        uint *elementos;
}

Conjunto alocaConjunto(uint tam);

void liberaConjunto(Conjunto conj);
