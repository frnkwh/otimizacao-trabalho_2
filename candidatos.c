#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <time.h>

typedef struct {
    int id;
    int *grupos;
    int num_grupos;
} Candidato;

typedef struct {
    Candidato *candidatos;
    int num_candidatos;
    int capacidade;
} CandidatoList;

#define FLAG_VIABILIDADE 'f'
#define FLAG_OTIMALIDADE 'o'
#define FLAG_FUNCAO_LIMITANTE 'a'

typedef struct {
    bool viabilidade;
    bool otimalidade;
    bool funcao_limitante;
} Flags;

// Função para criar um candidato
Candidato criarCandidato(int id, int *grupos, int num_grupos) {
    Candidato candidato;
    candidato.id = id;
    candidato.grupos = (int*) malloc(num_grupos * sizeof(int));
    for (int i = 0; i < num_grupos; i++) {
        candidato.grupos[i] = grupos[i];
    }
    candidato.num_grupos = num_grupos;
    return candidato;
}

// Função para inicializar uma lista de candidatos
CandidatoList criarCandidatoList(int capacidade) {
    CandidatoList lista;
    lista.candidatos = (Candidato*) malloc(capacidade * sizeof(Candidato));
    lista.num_candidatos = 0;
    lista.capacidade = capacidade;
    return lista;
}

// Função para adicionar um candidato à lista
void adicionarCandidato(CandidatoList *lista, Candidato candidato) {
    if (lista->num_candidatos >= lista->capacidade) {
        lista->capacidade *= 2;
        lista->candidatos = (Candidato*) realloc(lista->candidatos, lista->capacidade * sizeof(Candidato));
    }
    lista->candidatos[lista->num_candidatos++] = candidato;
}

// Função para imprimir a lista de candidatos para debug
void imprimirCandidatoListDebug(CandidatoList lista) {
    printf("Candidatos selecionados:\n");
    for (int i = 0; i < lista.num_candidatos; i++) {
        printf("Candidato %d: Grupos [", lista.candidatos[i].id);
        for (int j = 0; j < lista.candidatos[i].num_grupos; j++) {
            printf("%d", lista.candidatos[i].grupos[j]);
            if (j < lista.candidatos[i].num_grupos - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }
}

// Função para imprimir a lista de candidatos
void imprimirCandidatoList(CandidatoList lista) {
    for (int i = 0; i < lista.num_candidatos - 1; i++) {
        printf("%d ", lista.candidatos[i].id);
    }
    printf("%d\n", lista.candidatos[lista.num_candidatos - 1].id);
}

// Função para liberar a memória alocada pela lista de candidatos
void liberarCandidatoList(CandidatoList* lista) {
    for (int i = 0; i < lista->num_candidatos; i++) {
        free(lista->candidatos[i].grupos);
    }
    free(lista->candidatos);
    lista->num_candidatos = 0;
    lista->capacidade = 0;
}
