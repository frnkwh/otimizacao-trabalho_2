include "conjunto.h"

Conjunto alocaConjunto(uint tam) {
        Conjunto conj = malloc(sizeof(Conjunto));

        conj->tam = tam;
        conj->elementos = malloc(sizeof(uint) * tam);

        return conj;
}

void liberaConjunto(Conjunto conj) {
        free(conj->elementos);
        free(conj);
}
