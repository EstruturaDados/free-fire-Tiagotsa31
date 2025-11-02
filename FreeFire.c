#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20  // Limite de componentes

// ========================
// Definição da struct
// ========================
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;

// ========================
// Função para mostrar componentes
// ========================
void mostrarComponentes(Componente comp[], int total) {
    printf("\nComponentes:\n");
    for (int i = 0; i < total; i++) {
        printf("%d. Nome: %s | Tipo: %s | Prioridade: %d\n",
               i + 1, comp[i].nome, comp[i].tipo, comp[i].prioridade);
    }
}

// ========================
// Bubble Sort por nome
// ========================
int bubbleSortNome(Componente comp[], int total) {
    int comparacoes = 0;
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            comparacoes++;
            if (strcmp(comp[j].nome, comp[j + 1].nome) > 0) {
                Componente temp = comp[j];
                comp[j] = comp[j + 1];
                comp[j + 1] = temp;
            }
        }
    }
    return comparacoes;
}

// ========================
// Insertion Sort por tipo
// ========================
int insertionSortTipo(Componente comp[], int total) {
    int comparacoes = 0;
    for (int i = 1; i < total; i++) {
        Componente chave = comp[i];
        int j = i - 1;
        while (j >= 0 && strcmp(comp[j].tipo, chave.tipo) > 0) {
            comparacoes++;
            comp[j + 1] = comp[j];
            j--;
        }
        if (j >= 0) comparacoes++;
        comp[j + 1] = chave;
    }
    return comparacoes;
}

// ========================
// Selection Sort por prioridade
// ========================
int selectionSortPrioridade(Componente comp[], int total) {
    int comparacoes = 0;
    for (int i = 0; i < total - 1; i++) {
        int min = i;
        for (int j = i + 1; j < total; j++) {
            comparacoes++;
            if (comp[j].prioridade < comp[min].prioridade) {
                min = j;
            }
        }
        if (min != i) {
            Componente temp = comp[i];
            comp[i] = comp[min];
            comp[min] = temp;
        }
    }
    return comparacoes;
}

// ========================
// Busca binária por nome
// ========================
int buscaBinariaPorNome(Componente comp[], int total, char nome[], int* comparacoes) {
    int inicio = 0, fim = total - 1;
    *comparacoes = 0;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        (*comparacoes)++;
        int cmp = strcmp(comp[meio].nome, nome);
        if (cmp == 0) {
            return meio; // índice encontrado
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return -1; // não encontrado
}

// ========================
// Função para medir tempo de execução
// ========================
double medirTempo(int (*algoritmo)(Componente[], int), Componente comp[], int total, int* comparacoes) {
    clock_t inicio = clock();
    *comparacoes = algoritmo(comp, total);
    clock_t fim = clock();
    return ((double)(fim - inicio)) / CLOCKS_PER_SEC;
}

// ========================
// Função principal
// ========================
int main() {
    Componente torre[MAX_COMPONENTES];
    int total = 0;
    char chave[30]; // componente-chave
    int comparacoes;
    double tempo;

    printf("Cadastro de componentes da torre (max 20):\n");
    printf("Quantos componentes deseja cadastrar? ");
    scanf("%d", &total);
    getchar(); // limpar buffer

    if (total > MAX_COMPONENTES) total = MAX_COMPONENTES;

    for (int i = 0; i < total; i++) {
        printf("\nComponente %d:\n", i + 1);
        printf("Nome: ");
        fgets(torre[i].nome, 30, stdin);
        torre[i].nome[strcspn(torre[i].nome, "\n")] = 0; // remover \n

        printf("Tipo: ");
        fgets(torre[i].tipo, 20, stdin);
        torre[i].tipo[strcspn(torre[i].tipo, "\n")] = 0;

        printf("Prioridade (1 a 10): ");
        scanf("%d", &torre[i].prioridade);
        getchar();
    }

    printf("\nDigite o nome do componente-chave: ");
    fgets(chave, 30, stdin);
    chave[strcspn(chave, "\n")] = 0;

    int opcao;
    do {
        printf("\nEscolha a estrategia de ordenacao:\n");
        printf("1. Bubble Sort (por nome)\n");
        printf("2. Insertion Sort (por tipo)\n");
        printf("3. Selection Sort (por prioridade)\n");
        printf("0. Sair\nEscolha: ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            case 1:
                tempo = medirTempo(bubbleSortNome, torre, total, &comparacoes);
                printf("Componentes ordenados por nome (Bubble Sort):\n");
                mostrarComponentes(torre, total);
                printf("Comparacoes: %d | Tempo: %.6f s\n", comparacoes, tempo);

                // Busca binária do componente-chave
                int idx = buscaBinariaPorNome(torre, total, chave, &comparacoes);
                if (idx != -1) {
                    printf("Componente-chave encontrado! %s | Tipo: %s | Prioridade: %d\n",
                           torre[idx].nome, torre[idx].tipo, torre[idx].prioridade);
                    printf("Comparacoes na busca binaria: %d\n", comparacoes);
                } else {
                    printf("Componente-chave nao encontrado.\n");
                    printf("Comparacoes na busca binaria: %d\n", comparacoes);
                }
                break;
            case 2:
                tempo = medirTempo(insertionSortTipo, torre, total, &comparacoes);
                printf("Componentes ordenados por tipo (Insertion Sort):\n");
                mostrarComponentes(torre, total);
                printf("Comparacoes: %d | Tempo: %.6f s\n", comparacoes, tempo);
                break;
            case 3:
                tempo = medirTempo(selectionSortPrioridade, torre, total, &comparacoes);
                printf("Componentes ordenados por prioridade (Selection Sort):\n");
                mostrarComponentes(torre, total);
                printf("Comparacoes: %d | Tempo: %.6f s\n", comparacoes, tempo);
                break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n"); break;
        }
    } while(opcao != 0);

    return 0;
}
