#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int tam;
    int *grupos;
} Candidato;

typedef struct {
    int tamCandidatos;
    Candidato *candidatos;
} VetorCandidatos;

VetorCandidatos melhorSolucao;

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

//void liberaCandidato(Candidato c) {
//    if (c) {
//        free(c->grupos);
//        free(c);
//    }
//}

void liberaVetorCandidatos(VetorCandidatos v) {
    for (int i = 0; i < v.tamCandidatos; i++) {
        free(v.candidatos[i].grupos);
    }
    free(v.candidatos);
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
void insereSolucao(Candidato c, VetorCandidatos *solucao) {
    solucao->candidatos[solucao->tamCandidatos] = c;
    solucao->tamCandidatos++;
}

void removeUltimo(VetorCandidatos *solucao) {
    solucao->tamCandidatos--;
}

void copiaVetorCandidatos(VetorCandidatos *solucaoAtual) {
    for (int i = 0; i < solucaoAtual->tamCandidatos; i++) {
        melhorSolucao.candidatos[i] = solucaoAtual->candidatos[i];
    }
    melhorSolucao.tamCandidatos = solucaoAtual->tamCandidatos;
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

void semViabilidade(VetorCandidatos *vetorCandidatos, VetorCandidatos *solucaoAtual, int l, int n, int i) {
    Candidato temp;

    //nodosAcessados++; //sera?
    while (i < n) {
        temp = vetorCandidatos->candidatos[i];
        insereSolucao(temp, solucaoAtual); //passar i talvez
        semViabilidade(vetorCandidatos, solucaoAtual, l, n, i + 1);
        removeUltimo(solucaoAtual);
        //nodosAcessados++;
    }

    if(temTodosGrupos(solucaoAtual, l)) {
        if (solucaoAtual->tamCandidatos < melhorSolucao.tamCandidatos) {
            //talvez um memset pra zerar o vetor da melhorSolucao
            copiaVetorCandidatos(solucaoAtual);
        }
    }
}

int main() {
    int l, n, tam, g;

    if (scanf("%d %d", &l, &n) != 2) {
        fprintf(stderr, "Erro ao ler l e n.\n");
        return 1;
    }

    VetorCandidatos vetorCandidatos;
    vetorCandidatos.tamCandidatos = n;
    vetorCandidatos.candidatos = malloc(sizeof(Candidato) * n);

    for (int i = 0; i < n; i++) {
        if (scanf("%d", &tam) != 1) {
            fprintf(stderr, "Erro ao ler o tamanho do candidato %d.\n", i);
            return 1;
        }

        vetorCandidatos.candidatos[i].tam = tam;
        vetorCandidatos.candidatos[i].grupos = malloc(sizeof(int) * l);
        if (!vetorCandidatos.candidatos[i].grupos) {
            //for (int j = 0; j < i; j++) {
                //liberaCandidato(vetorCandidatos.candidatos[j]);
                free(vetorCandidatos.candidatos[i].grupos);
            //}
            free(vetorCandidatos.candidatos);
            return 1;
        }

        for (int j = 0; j < tam; j++) {
            if (scanf("%d", &g) != 1) {
                fprintf(stderr, "Erro ao ler o grupo %d do candidato %d.\n", j, i);
                //for (int k = 0; k <= i; k++) {
                //    liberaCandidato(vetorCandidatos.candidatos[k]);
                //}
                free(vetorCandidatos.candidatos);
                return 1;
            }
            vetorCandidatos.candidatos[i].grupos[j] = g;
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
    melhorSolucao.tamCandidatos = vetorCandidatos.tamCandidatos;
    melhorSolucao.candidatos = malloc(sizeof(Candidato) * n);

    if (!melhorSolucao.candidatos) {
        fprintf(stderr, "Erro ao alocar memória para melhorSolucao.\n");
        liberaVetorCandidatos(vetorCandidatos);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        melhorSolucao.candidatos[i].tam = vetorCandidatos.candidatos[i].tam;
        melhorSolucao.candidatos[i].grupos = malloc(sizeof(int) * l);
        if (!melhorSolucao.candidatos[i].grupos) {
            fprintf(stderr, "Erro ao alocar memória para grupos do melhorSolucao %d.\n", i);
            liberaVetorCandidatos(vetorCandidatos);
            liberaVetorCandidatos(melhorSolucao);
            return 1;
        }
        memcpy(melhorSolucao.candidatos[i].grupos, vetorCandidatos.candidatos[i].grupos, sizeof(int) * vetorCandidatos.candidatos[i].tam);
    }

    liberaVetorCandidatos(vetorCandidatos);
    liberaVetorCandidatos(melhorSolucao);
    //for (int i = 0; i < melhorSolucao.tamCandidatos; i++) {
    //    free(melhorSolucao.candidatos[i].grupos);
    //}
    //free(melhorSolucao.candidatos);

    return 0;
}

