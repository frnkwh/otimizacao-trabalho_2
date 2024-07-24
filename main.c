#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int tam;
    int *grupos;
} Candidato;

Candidato *melhorSolucao = NULL;

Candidato* alocaCandidato(int tam) {
    Candidato *c = malloc(sizeof(Candidato));
    if (!c) {
        fprintf(stderr, "Erro ao alocar memória para o candidato.\n");
        return NULL;
    }

    c->tam = tam;
    c->grupos = malloc(sizeof(int) * tam);
    if (!c->grupos) {
        fprintf(stderr, "Erro ao alocar memória para os grupos do candidato.\n");
        free(c);
        return NULL;
    }

    return c;
}

void liberaCandidato(Candidato *c) {
    if (c) {
        free(c->grupos);
        free(c);
    }
}

int checaRepresentados(int representado[], int l) {
    for (int i = 0; i < l; i++) {
        if (!representado[i]) // retorna 0 se algum grupo não está representado
            return 0;
    }
    return 1; // retorna 1 se todos os grupos estão representados
}

//int boundDada(int tamanhoAtual) {
    
//}

//void branchAndBound(int n, int l, Candidato **candidatos, int **representado, Candidato **solucaoAtual, int tamanhoAtual, Candidato **melhorSolucao, int *melhorTamanho) {
//    if (checaRepresentados(representado, l)) {
//        if (tamanhoAtual < melhorTamanho) {
//            *melhorTamanho = tamanhoAtual;
//            for (int i = 0; i < tamanhoAtual; i++) {
//                melhorSolucao[i] = solucaoAtual[i];
//            }
//        }
//    }

//    for (int i = 0; i < n; i++) {
//        int novosGrupos[l];
//        int countNovosGrupos = 0;
//        for (int j = 0; j < candidatos[i]->tam; j++) {
//            int grupo = candidatos[i]->grupos[j];
//            if (!representado[grupo]) {
//                representado[grupo] = 1;
//                novosGrupos[countNovosGrupos++] = grupo;
//            }
//        }
//        if (countNovosGrupos > 0) {
//            solucaoAtual[tamanhoAtual] = candidatos[i];
//            branchAndBound(n, l, candidatos, representado, solucaoAtual, tamanhoAtual + 1, melhorSolucao, melhorTamanho);
//
//            for (int j = 0; j < countNovosGrupos; j++)
//                representado[novosGrupos[j]] = 0;
//        }
//    }
//}

void semViabilidade(Candidato **candidatos, ) {
    Candidato *temp;

    nodosAcessados++; //sera?
    while (i < n) {
        temp = candidatos[i];
        insereSolucao(temp, solucaoAtual);
        semViabilidade(candidatos, solucaoAtual, n, i + 1);
        removeUltimo(solucaoAtual);
        nodosAcessados++;
    }

    if(temTodosGrupos(solucaoAtual)) {
        if (solucaoAtual->tam < melhorSolucao->tam) {
            //talvez um memset pra zerar o vetor da melhorSolucao
            copia(solucaoAtual, melhorSolucao);
            melhorSolucao->tam = solucaoAtual->tam
        }
    }
}

int main() {
    int l, n, tam, g;

    if (scanf("%d %d", &l, &n) != 2) {
        fprintf(stderr, "Erro ao ler l e n.\n");
        return 1;
    }
    int representado[l];

    if (!melhorSolucao) {
        fprintf(stderr, "Erro ao alocar memória para o array de melhorSolucao");
    }

    Candidato *candidatos = malloc(sizeof(Candidato) * n);
    if (!candidatos) {
        fprintf(stderr, "Erro ao alocar memória para o array de candidatos.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (scanf("%d", &tam) != 1) {
            fprintf(stderr, "Erro ao ler o tamanho do candidato %d.\n", i);
            return 1;
        }

        candidatos[i] = alocaCandidato(tam);
        if (!candidatos[i]) {
            for (int j = 0; j < i; j++) {
                liberaCandidato(candidatos[j]);
            }
            free(candidatos);
            return 1;
        }

        for (int j = 0; j < tam; j++) {
            if (scanf("%d", &g) != 1) {
                fprintf(stderr, "Erro ao ler o grupo %d do candidato %d.\n", j, i);
                for (int k = 0; k <= i; k++) {
                    liberaCandidato(candidatos[k]);
                }
                free(candidatos);
                return 1;
            }
            candidatos[i]->grupos[j] = g;
        }
    }

    melhorSolucao = malloc(sizeof(Candidato) * n);
    for (int i = 0, i < n; i++) {
        melhorSolucao[i] = candidatos[i];
    }
    melhorSolucao->tam = candidatos->tam;

    for (int i = 0; i < n; i++) {
        liberaCandidato(candidatos[i]);
    }

    free(candidatos);
    return 0;
}

