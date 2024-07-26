#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

typedef struct {
    int id;
    int* grupos;
    int num_grupos;
} Candidato;

typedef struct {
    Candidato* candidatos;
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

Flags flags = {true, true, true};

Candidato criarCandidato(int id, int* grupos, int num_grupos);
CandidatoList criarCandidatoList(int capacidade);
void adicionarCandidato(CandidatoList* lista, Candidato candidato);
bool cobrirTodosGrupos(CandidatoList* lista, int* gruposNecessarios, int numGruposNecessarios);
void encontrarMenorConjuntoBranchAndBound(CandidatoList* todos, CandidatoList* atual, CandidatoList* melhor, int* gruposNecessarios, int numGruposNecessarios, int index);
void imprimirCandidatoList(CandidatoList lista);
void liberarCandidatoList(CandidatoList* lista);
int Bdada(CandidatoList* atual, int* gruposNecessarios, int numGruposNecessarios);

// Função para criar um candidato
Candidato criarCandidato(int id, int* grupos, int num_grupos) {
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
void adicionarCandidato(CandidatoList* lista, Candidato candidato) {
    if (lista->num_candidatos >= lista->capacidade) {
        lista->capacidade *= 2;
        lista->candidatos = (Candidato*) realloc(lista->candidatos, lista->capacidade * sizeof(Candidato));
    }
    lista->candidatos[lista->num_candidatos++] = candidato;
}

// Função para verificar se todos os grupos necessários são cobertos pelos candidatos da lista
bool cobrirTodosGrupos(CandidatoList* lista, int* gruposNecessarios, int numGruposNecessarios) {
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

// Função de Branch and Bound para encontrar o menor conjunto de candidatos que cubra todos os grupos
void encontrarMenorConjuntoBranchAndBound(CandidatoList* todos, CandidatoList* atual, CandidatoList* melhor, int* gruposNecessarios, int numGruposNecessarios, int index) {
    if (index >= todos->num_candidatos) {
        return;
    }

    int bound = Bdada(atual, gruposNecessarios, numGruposNecessarios);
    if (melhor->num_candidatos != 0 && bound >= melhor->num_candidatos) {
        return; // Podar ramo
    }

    // Explora a solução incluindo o candidato atual
    adicionarCandidato(atual, todos->candidatos[index]);
    if (cobrirTodosGrupos(atual, gruposNecessarios, numGruposNecessarios)) {
        if (melhor->num_candidatos == 0 || atual->num_candidatos < melhor->num_candidatos) {
            liberarCandidatoList(melhor);
            *melhor = criarCandidatoList(atual->num_candidatos);
            for (int i = 0; i < atual->num_candidatos; i++) {
                Candidato candidatoAtual = atual->candidatos[i];
                int* grupos = (int*) malloc(candidatoAtual.num_grupos * sizeof(int));
                for (int j = 0; j < candidatoAtual.num_grupos; j++) {
                    grupos[j] = candidatoAtual.grupos[j];
                }
                adicionarCandidato(melhor, criarCandidato(candidatoAtual.id, grupos, candidatoAtual.num_grupos));
                free(grupos);
            }
        }
    }

    encontrarMenorConjuntoBranchAndBound(todos, atual, melhor, gruposNecessarios, numGruposNecessarios, index + 1);
    atual->num_candidatos--;
    encontrarMenorConjuntoBranchAndBound(todos, atual, melhor, gruposNecessarios, numGruposNecessarios, index + 1);
}

// Função para calcular a função limitante Bdada
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

// Função para imprimir a lista de candidatos
void imprimirCandidatoList(CandidatoList lista) {
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

// Função para liberar a memória alocada pela lista de candidatos
void liberarCandidatoList(CandidatoList* lista) {
    for (int i = 0; i < lista->num_candidatos; i++) {
        free(lista->candidatos[i].grupos);
    }
    free(lista->candidatos);
    lista->num_candidatos = 0;
    lista->capacidade = 0;
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
        int* grupos = (int*) malloc(s * sizeof(int));
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

    if (flags.funcao_limitante == false)
        encontrarMenorConjuntoBranchAndBound(&todosCandidatos, &candidatosAtuais, &melhorSolucao, gruposNecessarios, l, 0);

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

