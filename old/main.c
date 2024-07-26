#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    int tam;
    int *grupos;
} Candidato;

typedef struct {
    int tamCandidatos;
    Candidato *candidatos;
} VetorCandidatos;

Candidato alocaCandidato(int tam) {
    Candidato c;

    c.tam = tam;
    c.grupos = malloc(sizeof(int) * tam);
    if (!c.grupos) {
        fprintf(stderr, "Erro ao alocar memória para os grupos do candidato.\n");
        c.tam = 0;
        c.grupos = NULL;
    }

    return c;
}

Candidato *copiaCandidato(Candidato *candidato, int l) {
    Candidato *cp;
    int *g;

    cp = malloc(sizeof(Candidato));

    cp->tam = candidato->tam;
    cp->id = candidato->id;
    g = malloc(sizeof(int) * l);
    for (int i = 0; i < cp->tam; i++) {
        g[i] = candidato->grupos[i]; // Atribui o valor diretamente à posição do vetor
    }
    cp->grupos = g;

    return cp;
}

//void liberaCandidato(Candidato c) {
//    if (c) {
//        free(c->grupos);
//        free(c);
//    }
//}
/*
void liberaVetorCandidatos(VetorCandidatos *v, int n) {
    if(!v) return;
    for (int i = 0; i < n; i++) {
        if (v->candidatos[i].grupos) {
            free(v->candidatos[i].grupos);
        }
    }
    free(v->candidatos);
    free(v);
}
*/
void liberaVetorCandidatos(VetorCandidatos *v, int n) {
    if (!v) return;

    for (int i = 0; i < v->tamCandidatos; i++) {
        if (v->candidatos[i].grupos) {
            free(v->candidatos[i].grupos);
        }
    }
    free(v->candidatos);
    free(v);
}

int checaRepresentados(int representado[], int l) {
    for (int i = 0; i < l; i++) {
        if (!representado[i]) // retorna 0 se algum grupo não está representado
            return 0;
    }
    return 1; // retorna 1 se todos os grupos estão representados
}

void insereSolucao(Candidato c, VetorCandidatos *solucao, int i) {
    solucao->candidatos[i] = c;
    solucao->tamCandidatos++;
}

void removeUltimo(VetorCandidatos *solucao) {
    solucao->tamCandidatos--;
}
/*
void copiaVetorCandidatos(VetorCandidatos *solucaoAtual, VetorCandidatos *melhorSolucao) {
        for (int i = 0; i < solucaoAtual->tamCandidatos; i++) {
                melhorSolucao->candidatos[i].tam = solucaoAtual->candidatos[i].tam;
                melhorSolucao->candidatos[i].id = solucaoAtual->candidatos[i].id;
                melhorSolucao->candidatos[i].id = solucaoAtual->candidatos[i].id;
        }
        melhorSolucao->tamCandidatos = solucaoAtual->tamCandidatos;
}
*/
/*
void copiaVetorCandidatos(VetorCandidatos *solucaoAtual, VetorCandidatos *melhorSolucao) {
    // Alocar memória para candidatos em melhorSolucao se necessário
    if (melhorSolucao->candidatos == NULL) {
        melhorSolucao->candidatos = malloc(sizeof(Candidato) * solucaoAtual->tamCandidatos);
    }

    for (int i = 0; i < solucaoAtual->tamCandidatos; i++) {
        // Copiar o id e tam do candidato
        melhorSolucao->candidatos[i].id = solucaoAtual->candidatos[i].id;
        melhorSolucao->candidatos[i].tam = solucaoAtual->candidatos[i].tam;

        // Alocar memória para os grupos em melhorSolucao se necessário
        if (melhorSolucao->candidatos[i].grupos == NULL) {
            melhorSolucao->candidatos[i].grupos = malloc(sizeof(int) * solucaoAtual->candidatos[i].tam);
        }

        // Copiar os grupos usando memcpy
        memcpy(melhorSolucao->candidatos[i].grupos, solucaoAtual->candidatos[i].grupos, sizeof(int) * solucaoAtual->candidatos[i].tam);
    }

    melhorSolucao->tamCandidatos = solucaoAtual->tamCandidatos;
}
*/
void copiaVetorCandidatos(VetorCandidatos *solucaoAtual, VetorCandidatos *melhorSolucao, int l) {
    // Alocar memória para candidatos em melhorSolucao se necessário
    if (melhorSolucao->candidatos == NULL) {
        melhorSolucao->candidatos = malloc(sizeof(Candidato) * solucaoAtual->tamCandidatos);
    }

    // Copiar cada candidato usando a função copiaCandidato
    for (int i = 0; i < solucaoAtual->tamCandidatos; i++) {
        melhorSolucao->candidatos[i] = *copiaCandidato(&solucaoAtual->candidatos[i], l);
    }

    melhorSolucao->tamCandidatos = solucaoAtual->tamCandidatos;
}

int temTodosGrupos(VetorCandidatos *solucaoAtual, int l) {
    int representado[l];
    for (int i = 0; i < l; i++) {
        representado[i] = 0;
    }

    for (int i = 0; i < solucaoAtual->tamCandidatos; i++) {
        Candidato c = solucaoAtual->candidatos[i];
        for (int j = 0; j < c.tam; j++) {
            representado[c.grupos[j]] = 1;
        }
    }

    return checaRepresentados(representado, l);
}
/*
void semViabilidade(VetorCandidatos *vetorCandidatos, VetorCandidatos *solucaoAtual, VetorCandidatos *melhorSolucao, int l, int n, int i) {
    Candidato temp;

    printf("entrou pela %d vez \n", i + 1);

    if (i > n)
        return;
    //nodosAcessados++; //sera?
    while (i < n) {
        temp = vetorCandidatos->candidatos[i];
        insereSolucao(temp, solucaoAtual); //passar i talvez
        semViabilidade(vetorCandidatos, solucaoAtual, melhorSolucao, l, n, i + 1);
        removeUltimo(solucaoAtual);
        //nodosAcessados++;
    }

    if(temTodosGrupos(solucaoAtual, l)) {
        if (solucaoAtual->tamCandidatos < melhorSolucao->tamCandidatos) {
            //talvez um memset pra zerar o vetor da melhorSolucao
            copiaVetorCandidatos(solucaoAtual, melhorSolucao);
        }
    }
}
*/
void semViabilidade(VetorCandidatos *vetorCandidatos, VetorCandidatos *solucaoAtual, VetorCandidatos *melhorSolucao, int l, int n, int i) {
    Candidato temp;

    printf("entrou pela %d vez \n", i);

    while (i < n) {
        temp = vetorCandidatos->candidatos[i];
        insereSolucao(temp, solucaoAtual, i); //passar i talvez
        semViabilidade(vetorCandidatos, solucaoAtual, melhorSolucao, l, n, i + 1);
        removeUltimo(solucaoAtual);
    }

    if (temTodosGrupos(solucaoAtual, l)) {
        if (solucaoAtual->tamCandidatos < melhorSolucao->tamCandidatos) {
            //talvez um memset pra zerar o vetor da melhorSolucao
            copiaVetorCandidatos(solucaoAtual, melhorSolucao, l);
        }
    }
}
void imprimeVetorCandidatos(VetorCandidatos *v, int n) {
    for (int i = 0; i < n; i++) {
        int temp = v->candidatos[i].tam;
        printf("candidato %d\ntam %d\n", v->candidatos[i].id, temp);
        for (int j = 0; j < temp; j++) {
            printf("grupos %d\n", j + 1);
        }
        printf("\n");
    }
}

int main() {
    int l, n, tam, g;

    if (scanf("%d %d", &l, &n) != 2) {
        fprintf(stderr, "Erro ao ler l e n.\n");
        return 1;
    }

    VetorCandidatos *vetorCandidatos = NULL, *solucaoAtual = NULL, *melhorSolucao = NULL;
    vetorCandidatos = malloc(sizeof(VetorCandidatos));
    vetorCandidatos->candidatos = malloc(sizeof(Candidato) * n);
    vetorCandidatos->tamCandidatos = n;
    solucaoAtual = malloc(sizeof(VetorCandidatos));
    solucaoAtual->candidatos = malloc(sizeof(Candidato) * n);
    solucaoAtual->tamCandidatos = 0;

    for (int i = 0; i < n; i++) {
        if (scanf("%d", &tam) != 1) {
            fprintf(stderr, "Erro ao ler o tamanho do candidato %d.\n", i);
            return 1;
        }

        vetorCandidatos->candidatos[i].id = i + 1;
        vetorCandidatos->candidatos[i].tam = tam;
        vetorCandidatos->candidatos[i].grupos = malloc(sizeof(int) * l);
        solucaoAtual->candidatos[i].grupos = malloc(sizeof(int) * l);

        if (!vetorCandidatos->candidatos[i].grupos) {
            //for (int j = 0; j < i; j++) {
            //liberaCandidato(vetorCandidatos.candidatos[j]);
            free(vetorCandidatos->candidatos[i].grupos);
            //}
            free(vetorCandidatos->candidatos);
            return 1;
        }

        for (int j = 0; j < tam; j++) {
            if (scanf("%d", &g) != 1) {
                fprintf(stderr, "Erro ao ler o grupo %d do candidato %d.\n", j, i);
                //for (int k = 0; k <= i; k++) {
                //    liberaCandidato(vetorCandidatos.candidatos[k]);
                //}
                free(vetorCandidatos->candidatos);
                return 1;
            }
            vetorCandidatos->candidatos[i].grupos[j] = g;
        }
    }

    /*melhorSolucao.candidatos = malloc(sizeof(Candidato) * n);
    for (int i = 0; i < n; i++) {
        melhorSolucao.candidatos[i].grupos = malloc(sizeof(int) * l);
    }

    melhorSolucao.tamCandidatos = vetorCandidatos.tamCandidatos;
    for (int i = 0; i < n; i++) {
        melhorSolucao.candidatos[i] = vetorCandidatos.candidatos[i];
    }
    */
    melhorSolucao = malloc(sizeof(VetorCandidatos));
    melhorSolucao->tamCandidatos = vetorCandidatos->tamCandidatos;
    melhorSolucao->candidatos = malloc(sizeof(Candidato) * n);

    if (!melhorSolucao->candidatos) {
        fprintf(stderr, "Erro ao alocar memória para melhorSolucao.\n");
        liberaVetorCandidatos(vetorCandidatos, n);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        melhorSolucao->candidatos[i].id = vetorCandidatos->candidatos[i].id;
        melhorSolucao->candidatos[i].tam = vetorCandidatos->candidatos[i].tam;
        melhorSolucao->candidatos[i].grupos = malloc(sizeof(int) * vetorCandidatos->candidatos[i].tam);
        if (!melhorSolucao->candidatos[i].grupos) {
            // Liberar memória alocada até este ponto
            for (int j = 0; j < i; j++) {
                free(melhorSolucao->candidatos[j].grupos);
            }
            free(melhorSolucao->candidatos);
            free(melhorSolucao);
            fprintf(stderr, "Erro ao alocar memória para grupos do melhorSolucao %d.\n", i);
            liberaVetorCandidatos(vetorCandidatos, n);
            return 1;
        }
        memcpy(melhorSolucao->candidatos[i].grupos, vetorCandidatos->candidatos[i].grupos, sizeof(int) * vetorCandidatos->candidatos[i].tam);
    }

    /*
        for (int i = 0; i < n; i++) {
                int temp = vetorCandidatos->candidatos[i].tam;
                printf("candidato %d\ntam %d\n", vetorCandidatos->candidatos[i].id, temp);
                for (int j = 0; j < temp; j++) {
                        printf("grupos %d\n", j + 1);
                }
                printf("\n");
        }
*/
    imprimeVetorCandidatos(vetorCandidatos, n);

    //semViabilidade(vetorCandidatos, solucaoAtual, melhorSolucao, l, n, 0);

    liberaVetorCandidatos(vetorCandidatos, n);
    liberaVetorCandidatos(solucaoAtual, n);
    liberaVetorCandidatos(melhorSolucao, n);

    //for (int i = 0; i < melhorSolucao.tamCandidatos; i++) {
    //    free(melhorSolucao.candidatos[i].grupos);
    //}
    //free(melhorSolucao.candidatos);

    printf("tamanho da melhor solucao = %d\n", melhorSolucao->tamCandidatos);
    for (int i = 0; i < melhorSolucao->tamCandidatos; i++) {
        printf("candidatos: %d\n", melhorSolucao->candidatos[i].id);
    }

    return 0;
}

