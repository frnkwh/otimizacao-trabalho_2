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

Flags flags = {true, true, true};

int nosVisitados = 0;

Candidato criarCandidato(int id, int* grupos, int num_grupos);
CandidatoList criarCandidatoList(int capacidade);
void adicionarCandidato(CandidatoList* lista, Candidato candidato);
bool cobrirTodosGrupos(CandidatoList* lista, int* gruposNecessarios, int numGruposNecessarios);
void encontrarMenorConjuntoGeral(CandidatoList* todos, CandidatoList* atual, CandidatoList* melhor, int* gruposNecessarios, int numGruposNecessarios, int index);
void imprimirCandidatoList(CandidatoList lista);
void liberarCandidatoList(CandidatoList* lista);
int Bdada(CandidatoList* atual, int* gruposNecessarios, int numGruposNecessarios);
int Bminha(CandidatoList *atual, Candidato proximo, int *gruposNecessarios, int numGruposNecessarios); 

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

void encontrarMenorConjuntoGeral(CandidatoList* todos, CandidatoList* atual, CandidatoList* melhor, int* gruposNecessarios, int numGruposNecessarios, int index) {
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
    printf("Tempo: %f segundos\n", tempoGasto);
    printf("Nós visitados: %d\n", nosVisitados);

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

