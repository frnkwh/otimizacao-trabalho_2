#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <time.h>
#include "candidatos.h"

Flags flags = {true, true, true};

int nosVisitados = 0;

// Função para verificar se todos os grupos necessários são cobertos pelos candidatos da lista
bool cobrirTodosGrupos(CandidatoList *lista, int *gruposNecessarios, int numGruposNecessarios) {
    bool* cobertos = (bool*) malloc(numGruposNecessarios * sizeof(bool));
    for (int i = 0; i < numGruposNecessarios; i++) {
        cobertos[i] = false;
    }

    for (int i = 0; i < lista->num_candidatos; i++) {
        for (int j = 0; j < lista->candidatos[i].num_grupos; j++) {
            for (int k = 0; k < numGruposNecessarios; k++) {
                if (lista->candidatos[i].grupos[j] == gruposNecessarios[k]) {
                    cobertos[k] = true;
                }
            }
        }
    }

    for (int i = 0; i < numGruposNecessarios; i++) {
        if (!cobertos[i]) {
            free(cobertos);
            return false;
        }
    }

    free(cobertos);
    return true;
}

int Bdada(CandidatoList* atual, int* gruposNecessarios, int numGruposNecessarios) {
    int countCobertos = 0;
    bool* cobertos = (bool*) malloc(numGruposNecessarios * sizeof(bool));
    for (int i = 0; i < numGruposNecessarios; i++) {
        cobertos[i] = false;
    }

    for (int i = 0; i < atual->num_candidatos; i++) {
        for (int j = 0; j < atual->candidatos[i].num_grupos; j++) {
            for (int k = 0; k < numGruposNecessarios; k++) {
                if (atual->candidatos[i].grupos[j] == gruposNecessarios[k]) {
                    cobertos[k] = true;
                }
            }
        }
    }

    for (int i = 0; i < numGruposNecessarios; i++) {
        if (cobertos[i]) {
            countCobertos++;
        }
    }

    free(cobertos);
    return atual->num_candidatos + (countCobertos == numGruposNecessarios ? 0 : 1);
}

int Bminha(CandidatoList *atual, Candidato proximo, int *gruposNecessarios, int numGruposNecessarios) {
    int countCobertos = 0;
    bool* cobertos = (bool*) malloc(numGruposNecessarios * sizeof(bool));
    for (int i = 0; i < numGruposNecessarios; i++) {
        cobertos[i] = false;
    }

    for (int i = 0; i < atual->num_candidatos; i++) {
        for (int j = 0; j < atual->candidatos[i].num_grupos; j++) {
            for (int k = 0; k < numGruposNecessarios; k++) {
                if (atual->candidatos[i].grupos[j] == gruposNecessarios[k]) {
                    cobertos[k] = true;
                }
            }
        }
    }

    for (int i = 0; i < numGruposNecessarios; i++) {
        if (cobertos[i]) {
            countCobertos++;
        }
    }

    int gruposRestantes = numGruposNecessarios - countCobertos;
    free(cobertos);
    //printf("num_grupos do proximo candidato: %d\n", num_grupos);
    //printf("numGruposNecessarios: %d\n", numGruposNecessarios);
    //printf("\n");

    if (gruposRestantes == 0) {
        //printf("+0\n");
        return atual->num_candidatos;
    }

    if (proximo.num_grupos >= gruposRestantes) {
        //printf("+1\n");
        return atual->num_candidatos + 1;
    } else {
        //printf("+2\n");
        return atual->num_candidatos + 2;
    }
}

void encontrarMenorConjuntoGeral(CandidatoList *todos, CandidatoList *atual, CandidatoList *melhor, int *gruposNecessarios, int numGruposNecessarios, int index) {
    nosVisitados++;

    if (index >= todos->num_candidatos) {
        return;
    }

    if (!flags.viabilidade && index == todos->num_candidatos) {
        if (cobrirTodosGrupos(atual, gruposNecessarios, numGruposNecessarios)) {
            if (melhor->num_candidatos == 0 || atual->num_candidatos < melhor->num_candidatos) {
                liberarCandidatoList(melhor); // Liberar a lista anterior antes de substituir
                *melhor = criarCandidatoList(atual->num_candidatos);
                for (int i = 0; i < atual->num_candidatos; i++) {
                    Candidato candidatoAtual = atual->candidatos[i];
                    int* grupos = (int*) malloc(candidatoAtual.num_grupos * sizeof(int));
                    for (int j = 0; j < candidatoAtual.num_grupos; j++) {
                        grupos[j] = candidatoAtual.grupos[j];
                    }
                    adicionarCandidato(melhor, criarCandidato(candidatoAtual.id, grupos, candidatoAtual.num_grupos));
                    free(grupos); // Liberar memória temporária
                }
            }
        }
        return;
    }

    if (flags.otimalidade) {
        int bound;
        if (index < todos->num_candidatos) {
            if (!flags.funcao_limitante) {
                /*
                if (index == 0)
                    insertionSortInverso(todos);
                */
                bound = Bdada(atual, gruposNecessarios, numGruposNecessarios);
            } else {
                bound = Bminha(atual, todos->candidatos[index], gruposNecessarios, numGruposNecessarios);
            }
            if (melhor->num_candidatos != 0 && bound >= melhor->num_candidatos) {
                return; // Podar ramo
            }
            adicionarCandidato(atual, todos->candidatos[index]);
        }
    }

    if (cobrirTodosGrupos(atual, gruposNecessarios, numGruposNecessarios)) {
        if (melhor->num_candidatos == 0 || atual->num_candidatos < melhor->num_candidatos) {
            liberarCandidatoList(melhor); // Liberar a lista anterior antes de substituir
            *melhor = criarCandidatoList(atual->num_candidatos);
            for (int i = 0; i < atual->num_candidatos; i++) {
                Candidato candidatoAtual = atual->candidatos[i];
                int* grupos = (int*) malloc(candidatoAtual.num_grupos * sizeof(int));
                for (int j = 0; j < candidatoAtual.num_grupos; j++) {
                    grupos[j] = candidatoAtual.grupos[j];
                }
                adicionarCandidato(melhor, criarCandidato(candidatoAtual.id, grupos, candidatoAtual.num_grupos));
                free(grupos); // Liberar memória temporária
            }
        }
        if (!flags.otimalidade)
            return;
    }

    if (index < todos->num_candidatos) {
        if (!flags.otimalidade) {
            adicionarCandidato(atual, todos->candidatos[index]);
        }

        encontrarMenorConjuntoGeral(todos, atual, melhor, gruposNecessarios, numGruposNecessarios, index + 1);
        atual->num_candidatos--;
    }

    encontrarMenorConjuntoGeral(todos, atual, melhor, gruposNecessarios, numGruposNecessarios, index + 1);
}

int main(int argc, char *argv[]) {
    int l, n, opt;
    scanf("%d %d", &l, &n);

    while((opt = getopt(argc, argv, "foa")) != -1) {
        switch(opt) {
            case FLAG_VIABILIDADE:
                flags.viabilidade = false;
                break;
            case FLAG_OTIMALIDADE:
                flags.otimalidade = false;
                break;
            case FLAG_FUNCAO_LIMITANTE:
                flags.funcao_limitante = false;
                break;
        }
    }

    CandidatoList todosCandidatos = criarCandidatoList(n);

    for (int i = 1; i <= n; i++) {
        int s;
        scanf("%d", &s);
        int *grupos = (int*) malloc(s * sizeof(int));
        for (int j = 0; j < s; j++) {
            scanf("%d", &grupos[j]);
        }
        Candidato candidato = criarCandidato(i, grupos, s);
        adicionarCandidato(&todosCandidatos, candidato);
        free(grupos);
    }

    int* gruposNecessarios = (int*) malloc(l * sizeof(int));
    for (int i = 0; i < l; i++) {
        gruposNecessarios[i] = i + 1;
    }

    CandidatoList melhorSolucao = criarCandidatoList(n);
    CandidatoList candidatosAtuais = criarCandidatoList(n);

    clock_t inicio, fim;
    double tempoGasto;

    inicio = clock();
    encontrarMenorConjuntoGeral(&todosCandidatos, &candidatosAtuais, &melhorSolucao, gruposNecessarios, l, 0);
    fim = clock();
    tempoGasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    fprintf(stderr, "Tempo: %f segundos\n", tempoGasto);
    fprintf(stderr, "Nós visitados: %d\n", nosVisitados);

    if (melhorSolucao.num_candidatos == 0) {
        printf("Inviavel\n");
    } else {
        imprimirCandidatoList(melhorSolucao);
    }

    liberarCandidatoList(&todosCandidatos);
    liberarCandidatoList(&melhorSolucao);
    liberarCandidatoList(&candidatosAtuais);
    free(gruposNecessarios);

    return 0;
}

