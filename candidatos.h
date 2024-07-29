#ifndef CANDIDATOS_H
#define CANDIDATOS_H

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
Candidato criarCandidato(int id, int *grupos, int num_grupos);

// Função para inicializar uma lista de candidatos
CandidatoList criarCandidatoList(int capacidade);

// Função para adicionar um candidato à lista
void adicionarCandidato(CandidatoList *lista, Candidato candidato);

// Função para imprimir a lista de candidatos para debug
void imprimirCandidatoListDebug(CandidatoList lista);

// Função para imprimir a lista de candidatos
void imprimirCandidatoList(CandidatoList lista);

// Função para liberar a memória alocada pela lista de candidatos
void liberarCandidatoList(CandidatoList* lista);

#endif
