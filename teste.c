#include <stdio.h>
#include <stdlib.h>

#define MAX_SETS 100
#define MAX_ELEMENTS 100

typedef struct {
    int elements[MAX_ELEMENTS];
    int size;
} Set;

Set best_solution[MAX_SETS];
int best_solution_size = MAX_SETS;

int is_covered(int universe_size, int covered[]) {
    for (int i = 0; i < universe_size; i++) {
        if (!covered[i]) {
            return 0;
        }
    }
    return 1;
}

void backtrack(int universe_size, int subsets_size, Set subsets[], int covered[], Set current_solution[], int current_solution_size) {
    if (is_covered(universe_size, covered)) {
        if (current_solution_size < best_solution_size) {
            best_solution_size = current_solution_size;
            for (int i = 0; i < current_solution_size; i++) {
                best_solution[i] = current_solution[i];
            }
        }
        return;
    }

    for (int i = 0; i < subsets_size; i++) {
        int new_elements[MAX_ELEMENTS] = {0};
        int new_elements_count = 0;
        for (int j = 0; j < subsets[i].size; j++) {
            int element = subsets[i].elements[j];
            if (!covered[element]) {
                covered[element] = 1;
                new_elements[new_elements_count++] = element;
            }
        }
        if (new_elements_count > 0) {
            current_solution[current_solution_size] = subsets[i];
            backtrack(universe_size, subsets_size, subsets, covered, current_solution, current_solution_size + 1);
            for (int j = 0; j < new_elements_count; j++) {
                covered[new_elements[j]] = 0;
            }
        }
    }
}

int main() {
    int universe_size = 5;
    int subsets_size = 5;
    Set subsets[MAX_SETS] = {
        {{0, 1, 2}, 3},
        {{1, 3}, 2},
        {{2, 3}, 2},
        {{2, 4}, 2},
        {{4}, 1}
    };
    int covered[MAX_ELEMENTS] = {0};
    Set current_solution[MAX_SETS];
    best_solution_size = subsets_size;

    backtrack(universe_size, subsets_size, subsets, covered, current_solution, 0);

    printf("The smallest set cover is:\n");
    for (int i = 0; i < best_solution_size; i++) {
        printf("{ ");
        for (int j = 0; j < best_solution[i].size; j++) {
            printf("%d ", best_solution[i].elements[j]);
        }
        printf("}\n");
    }
    return 0;
}

