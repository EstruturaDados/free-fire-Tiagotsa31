/*
  main.c
  Sistema de inventário (vetor + lista encadeada) e módulo de componentes da torre.
  - Implementa structs Item e Componente.
  - Vetor com capacidade 10 (inventário inicial).
  - Lista encadeada equivalente.
  - Operações: inserir, remover por nome, listar, busca sequencial.
  - Ordenação no vetor (por nome) e busca binária com contagem de comparações.
  - Módulo de componentes (até 20) com bubble/insertion/selection sorts,
    contagem de comparações e medição de tempo (clock()).
  Bibliotecas: stdio.h, stdlib.h, string.h, time.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS_VETOR 10
#define MAX_NOME 30
#define MAX_TIPO 20
#define MAX_COMPONENTES 20
#define MAX_BUFFER 100

/* -----------------------
   Structs principais
   ----------------------- */

/* Item representa um objeto coletado (nome, tipo, quantidade) */
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
} Item;

/* No para lista encadeada contendo um Item */
typedef struct No {
    Item dados;
    struct No* proximo;
} No;

/* Componente para a fase da torre */
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int prioridade; /* 1 a 10 */
} Componente;

/* -----------------------
   Variáveis / Contadores
   ----------------------- */

/* Vetor de itens (mochila sequencial) */
Item mochilaVetor[MAX_ITENS_VETOR];
int qtdVetor = 0;

/* Lista encadeada (mochila dinâmica) */
No* cabecaLista = NULL;

/* Contadores de comparações */
long comparacoesBuscaSequencialVetor = 0;
long comparacoesBuscaBinariaVetor = 0;
long comparacoesBuscaSequencialLista = 0;

/* -----------------------
   Funções utilitárias
   ----------------------- */

/* Limpa o '\n' restante de fgets */
void strip_newline(char* str) {
    size_t len = strlen(str);
    if (len == 0) return;
    if (str[len-1] == '\n') str[len-1] = '\0';
}

/* Le uma string com fgets em buffer seguro e copia para destino */
void lerString(const char* prompt, char* destino, int tamanho) {
    char buffer[MAX_BUFFER];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        buffer[0] = '\0';
    }
    strip_newline(buffer);
    /* copia com segurança */
    strncpy(destino, buffer, tamanho-1);
    destino[tamanho-1] = '\0';
}

/* Le um inteiro de forma segura */
int lerInteiro(const char* prompt) {
    char buffer[MAX_BUFFER];
    int valor;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
        if (sscanf(buffer, "%d", &valor) == 1) return valor;
        printf("Entrada inválida. Tente novamente.\n");
    }
}

/* Exibe um Item formatado */
void imprimirItem(const Item* it) {
    printf("Nome: %-28s | Tipo: %-10s | Quantidade: %d\n", it->nome, it->tipo, it->quantidade);
}

/* -----------------------
   Operações - VETOR
   ----------------------- */

/* Inserir item no vetor (até MAX_ITENS_VETOR) */
void inserirItemVetor() {
    if (qtdVetor >= MAX_ITENS_VETOR) {
        printf("Mochila (vetor) cheia. Remova um item antes de inserir.\n");
        return;
    }
    Item novo;
    lerString("Nome do item: ", novo.nome, MAX_NOME);
    lerString("Tipo (ex: arma, munição, cura): ", novo.tipo, MAX_TIPO);
    novo.quantidade = lerInteiro("Quantidade: ");
    /* inserir ao final */
    mochilaVetor[qtdVetor++] = novo;
    printf("Item inserido com sucesso na mochila (vetor).\n");
    /* listar após operação */
    printf("Itens atuais (vetor):\n");
    for (int i = 0; i < qtdVetor; i++) {
        printf(" %d) ", i+1);
        imprimirItem(&mochilaVetor[i]);
    }
}

/* Remover item por nome no vetor */
void removerItemVetor() {
    if (qtdVetor == 0) {
        printf("Mochila (vetor) vazia.\n");
        return;
    }
    char nome[MAX_NOME];
    lerString("Informe o nome do item a remover: ", nome, MAX_NOME);
    int i, encontrado = -1;
    for (i = 0; i < qtdVetor; i++) {
        if (strcmp(mochilaVetor[i].nome, nome) == 0) {
            encontrado = i;
            break;
        }
    }
    if (encontrado == -1) {
        printf("Item '%s' não encontrado na mochila (vetor).\n", nome);
    } else {
        /* desloca elementos à esquerda */
        for (i = encontrado; i < qtdVetor-1; i++) {
            mochilaVetor[i] = mochilaVetor[i+1];
        }
        qtdVetor--;
        printf("Item '%s' removido com sucesso (vetor).\n", nome);
    }
    /* listar após operação */
    printf("Itens atuais (vetor):\n");
    for (i = 0; i < qtdVetor; i++) {
        printf(" %d) ", i+1);
        imprimirItem(&mochilaVetor[i]);
    }
}

/* Listar itens do vetor */
void listarItensVetor() {
    printf("Listando itens na mochila (vetor) — total: %d\n", qtdVetor);
    for (int i = 0; i < qtdVetor; i++) {
        printf(" %d) ", i+1);
        imprimirItem(&mochilaVetor[i]);
    }
}

/* Busca sequencial por nome no vetor — incrementa contador global */
int buscarSequencialVetor(const char* nomeBuscado) {
    comparacoesBuscaSequencialVetor = 0;
    for (int i = 0; i < qtdVetor; i++) {
        comparacoesBuscaSequencialVetor++;
        if (strcmp(mochilaVetor[i].nome, nomeBuscado) == 0) {
            printf("Item encontrado na mochila (vetor) após %ld comparações:\n", comparacoesBuscaSequencialVetor);
            imprimirItem(&mochilaVetor[i]);
            return i;
        }
    }
    printf("Item '%s' NÃO encontrado na mochila (vetor). Comparações: %ld\n", nomeBuscado, comparacoesBuscaSequencialVetor);
    return -1;
}

/* Ordenação do vetor por nome (bubble sort) — também exibe comparações */
void ordenarVetorPorNome() {
    if (qtdVetor <= 1) {
        printf("Nada a ordenar (vetor possui %d item(s)).\n", qtdVetor);
        return;
    }
    long comparacoes = 0;
    int n = qtdVetor;
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-1-i; j++) {
            comparacoes++;
            if (strcmp(mochilaVetor[j].nome, mochilaVetor[j+1].nome) > 0) {
                Item tmp = mochilaVetor[j];
                mochilaVetor[j] = mochilaVetor[j+1];
                mochilaVetor[j+1] = tmp;
            }
        }
    }
    printf("Vetor ordenado por nome (bubble sort). Comparações realizadas: %ld\n", comparacoes);
    listarItensVetor();
}

/* Busca binária por nome no vetor — pressupõe vetor ordenado por nome */
int buscarBinariaVetor(const char* nomeBuscado) {
    comparacoesBuscaBinariaVetor = 0;
    int low = 0, high = qtdVetor - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        comparacoesBuscaBinariaVetor++;
        int cmp = strcmp(mochilaVetor[mid].nome, nomeBuscado);
        if (cmp == 0) {
            printf("Busca binária encontrou o item em índice %d após %ld comparações:\n", mid, comparacoesBuscaBinariaVetor);
            imprimirItem(&mochilaVetor[mid]);
            return mid;
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    printf("Busca binária: item '%s' NÃO encontrado. Comparações: %ld\n", nomeBuscado, comparacoesBuscaBinariaVetor);
    return -1;
}

/* -----------------------
   Operações - LISTA ENCADEADA
   ----------------------- */

/* Inserir item na lista (ao final) */
void inserirItemLista() {
    Item novo;
    lerString("Nome do item: ", novo.nome, MAX_NOME);
    lerString("Tipo (ex: arma, munição, cura): ", novo.tipo, MAX_TIPO);
    novo.quantidade = lerInteiro("Quantidade: ");
    No* novoNo = (No*)malloc(sizeof(No));
    if (!novoNo) {
        printf("Erro de memória.\n");
        return;
    }
    novoNo->dados = novo;
    novoNo->proximo = NULL;
    if (cabecaLista == NULL) {
        cabecaLista = novoNo;
    } else {
        No* ptr = cabecaLista;
        while (ptr->proximo) ptr = ptr->proximo;
        ptr->proximo = novoNo;
    }
    printf("Item inserido com sucesso na mochila (lista encadeada).\n");
    /* listar após operação */
    printf("Itens atuais (lista):\n");
    No* ptr = cabecaLista;
    int idx = 1;
    while (ptr) {
        printf(" %d) ", idx++);
        imprimirItem(&ptr->dados);
        ptr = ptr->proximo;
    }
}

/* Remover item por nome na lista */
void removerItemLista() {
    if (!cabecaLista) {
        printf("Mochila (lista) vazia.\n");
        return;
    }
    char nome[MAX_NOME];
    lerString("Informe o nome do item a remover: ", nome, MAX_NOME);
    No* atual = cabecaLista;
    No* anterior = NULL;
    while (atual) {
        if (strcmp(atual->dados.nome, nome) == 0) break;
        anterior = atual;
        atual = atual->proximo;
    }
    if (!atual) {
        printf("Item '%s' não encontrado na mochila (lista).\n", nome);
    } else {
        if (!anterior) { /* remover cabeça */
            cabecaLista = atual->proximo;
        } else {
            anterior->proximo = atual->proximo;
        }
        free(atual);
        printf("Item '%s' removido com sucesso (lista).\n", nome);
    }
    /* listar após operação */
    printf("Itens atuais (lista):\n");
    No* ptr = cabecaLista;
    int idx = 1;
    while (ptr) {
        printf(" %d) ", idx++);
        imprimirItem(&ptr->dados);
        ptr = ptr->proximo;
    }
}

/* Listar itens da lista */
void listarItensLista() {
    printf("Listando itens na mochila (lista):\n");
    No* ptr = cabecaLista;
    int idx = 1;
    while (ptr) {
        printf(" %d) ", idx++);
        imprimirItem(&ptr->dados);
        ptr = ptr->proximo;
    }
    if (idx == 1) printf(" (vazia)\n");
}

/* Busca sequencial na lista por nome — conta comparações */
No* buscarSequencialLista(const char* nomeBuscado) {
    comparacoesBuscaSequencialLista = 0;
    No* ptr = cabecaLista;
    while (ptr) {
        comparacoesBuscaSequencialLista++;
        if (strcmp(ptr->dados.nome, nomeBuscado) == 0) {
            printf("Item encontrado na lista após %ld comparações:\n", comparacoesBuscaSequencialLista);
            imprimirItem(&ptr->dados);
            return ptr;
        }
        ptr = ptr->proximo;
    }
    printf("Item '%s' NÃO encontrado na lista. Comparações: %ld\n", nomeBuscado, comparacoesBuscaSequencialLista);
    return NULL;
}

/* -----------------------
   Módulo - COMPONENTES DA TORRE
   ----------------------- */

/* Mostra componentes formatados */
void mostrarComponentes(Componente comps[], int n) {
    if (n == 0) {
        printf("Nenhum componente cadastrado.\n");
        return;
    }
    printf("Componentes (total %d):\n", n);
    for (int i = 0; i < n; i++) {
        printf(" %2d) Nome: %-25s | Tipo: %-12s | Prioridade: %d\n", i+1, comps[i].nome, comps[i].tipo, comps[i].prioridade);
    }
}

/* Bubble sort por nome (string) — conta comparações e retorna comparações */
long bubbleSortNome(Componente comps[], int n) {
    long comparacoes = 0;
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-1-i; j++) {
            comparacoes++;
            if (strcmp(comps[j].nome, comps[j+1].nome) > 0) {
                Componente tmp = comps[j];
                comps[j] = comps[j+1];
                comps[j+1] = tmp;
            }
        }
    }
    return comparacoes;
}

/* Insertion sort por tipo (string) */
long insertionSortTipo(Componente comps[], int n) {
    long comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente chave = comps[i];
        int j = i - 1;
        while (j >= 0) {
            comparacoes++;
            if (strcmp(comps[j].tipo, chave.tipo) > 0) {
                comps[j+1] = comps[j];
                j--;
            } else break;
        }
        comps[j+1] = chave;
    }
    return comparacoes;
}

/* Selection sort por prioridade (int) */
long selectionSortPrioridade(Componente comps[], int n) {
    long comparacoes = 0;
    for (int i = 0; i < n-1; i++) {
        int minIdx = i;
        for (int j = i+1; j < n; j++) {
            comparacoes++;
            if (comps[j].prioridade < comps[minIdx].prioridade) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            Componente tmp = comps[i];
            comps[i] = comps[minIdx];
            comps[minIdx] = tmp;
        }
    }
    return comparacoes;
}

/* Busca binária por nome em componentes (pressupõe ordenado por nome) */
int buscaBinariaPorNome(Componente comps[], int n, const char* alvo, long* comparacoes) {
    int low = 0, high = n - 1;
    *comparacoes = 0;
    while (low <= high) {
        int mid = (low + high) / 2;
        (*comparacoes)++;
        int cmp = strcmp(comps[mid].nome, alvo);
        if (cmp == 0) return mid;
        else if (cmp < 0) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

/* Wrapper para medir tempo de execução de cada algoritmo de ordenação */
double medirTempoOrdenacao(long (*algoritmo)(Componente[], int), Componente comps[], int n, long* comparacoes) {
    Componente copia[MAX_COMPONENTES];
    for (int i = 0; i < n; i++) copia[i] = comps[i]; /* preservar original */
    clock_t inicio = clock();
    *comparacoes = algoritmo(copia, n);
    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    /* exibir resultado e mostrar vetor ordenado */
    printf("Resultado da ordenação (comparações: %ld, tempo: %.6f segundos):\n", *comparacoes, tempo);
    mostrarComponentes(copia, n);
    return tempo;
}

/* -----------------------
   Menus e fluxo principal
   ----------------------- */

/* Menu para operações no vetor */
void menuVetor() {
    while (1) {
        printf("\n--- Mochila (Vetor) ---\n");
        printf("1) Inserir item\n2) Remover item por nome\n3) Listar itens\n4) Buscar sequencial por nome\n5) Ordenar por nome (bubble sort)\n6) Busca binária (requer vetor ordenado)\n0) Voltar\nEscolha: ");
        int op = lerInteiro("");
        if (op == 1) inserirItemVetor();
        else if (op == 2) removerItemVetor();
        else if (op == 3) listarItensVetor();
        else if (op == 4) {
            char nome[MAX_NOME];
            lerString("Nome a buscar: ", nome, MAX_NOME);
            buscarSequencialVetor(nome);
        } else if (op == 5) ordenarVetorPorNome();
        else if (op == 6) {
            char nome[MAX_NOME];
            lerString("Nome a buscar (binária): ", nome, MAX_NOME);
            buscarBinariaVetor(nome);
        } else if (op == 0) break;
        else printf("Opção inválida.\n");
    }
}

/* Menu para operações na lista encadeada */
void menuLista() {
    while (1) {
        printf("\n--- Mochila (Lista Encadeada) ---\n");
        printf("1) Inserir item\n2) Remover item por nome\n3) Listar itens\n4) Buscar sequencial por nome\n0) Voltar\nEscolha: ");
        int op = lerInteiro("");
        if (op == 1) inserirItemLista();
        else if (op == 2) removerItemLista();
        else if (op == 3) listarItensLista();
        else if (op == 4) {
            char nome[MAX_NOME];
            lerString("Nome a buscar: ", nome, MAX_NOME);
            buscarSequencialLista(nome);
        } else if (op == 0) break;
        else printf("Opção inválida.\n");
    }
}

/* Menu para módulos de comparação e ordenação (componentes da torre) */
void menuComponentes() {
    Componente comps[MAX_COMPONENTES];
    int n = 0;
    while (1) {
        printf("\n--- Módulo de Componentes da Torre ---\n");
        printf("1) Cadastrar componente (até %d)\n2) Listar componentes\n3) Bubble Sort por nome (mostrar comparações/tempo)\n4) Insertion Sort por tipo (mostrar comparações/tempo)\n5) Selection Sort por prioridade (mostrar comparações/tempo)\n6) Busca binária por nome (requer ordenação por nome)\n0) Voltar\nEscolha: ");
        int op = lerInteiro("");
        if (op == 1) {
            if (n >= MAX_COMPONENTES) {
                printf("Limite de componentes atingido.\n");
            } else {
                lerString("Nome do componente: ", comps[n].nome, MAX_NOME);
                lerString("Tipo (ex: controle, suporte, propulsão): ", comps[n].tipo, MAX_TIPO);
                int p;
                while (1) {
                    p = lerInteiro("Prioridade (1-10): ");
                    if (p >= 1 && p <= 10) break;
                    printf("Prioridade inválida. Use 1 a 10.\n");
                }
                comps[n].prioridade = p;
                n++;
                printf("Componente cadastrado.\n");
                mostrarComponentes(comps, n);
            }
        } else if (op == 2) {
            mostrarComponentes(comps, n);
        } else if (op == 3) {
            if (n <= 1) printf("Nada a ordenar.\n");
            else {
                Componente copia[MAX_COMPONENTES];
                for (int i = 0; i < n; i++) copia[i] = comps[i];
                clock_t inicio = clock();
                long cmp = bubbleSortNome(copia, n);
                clock_t fim = clock();
                double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
                printf("Bubble Sort por nome -> comparações: %ld, tempo: %.6f s\n", cmp, tempo);
                mostrarComponentes(copia, n);
            }
        } else if (op == 4) {
            if (n <= 1) printf("Nada a ordenar.\n");
            else {
                Componente copia[MAX_COMPONENTES];
                for (int i = 0; i < n; i++) copia[i] = comps[i];
                clock_t inicio = clock();
                long cmp = insertionSortTipo(copia, n);
                clock_t fim = clock();
                double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
                printf("Insertion Sort por tipo -> comparações: %ld, tempo: %.6f s\n", cmp, tempo);
                mostrarComponentes(copia, n);
            }
        } else if (op == 5) {
            if (n <= 1) printf("Nada a ordenar.\n");
            else {
                Componente copia[MAX_COMPONENTES];
                for (int i = 0; i < n; i++) copia[i] = comps[i];
                clock_t inicio = clock();
                long cmp = selectionSortPrioridade(copia, n);
                clock_t fim = clock();
                double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
                printf("Selection Sort por prioridade -> comparações: %ld, tempo: %.6f s\n", cmp, tempo);
                mostrarComponentes(copia, n);
            }
        } else if (op == 6) {
            if (n == 0) {
                printf("Nenhum componente cadastrado.\n");
            } else {
                /* Para busca binária, pedimos que o jogador ordene por nome primeiro.
                   Aqui podemos ordenar temporariamente por nome e executar a busca. */
                Componente copia[MAX_COMPONENTES];
                for (int i = 0; i < n; i++) copia[i] = comps[i];
                long cmpOrdenacao = bubbleSortNome(copia, n);
                printf("Vetor ordenado por nome (para busca binária). Comparações na ordenação: %ld\n", cmpOrdenacao);
                mostrarComponentes(copia, n);
                char alvo[MAX_NOME];
                lerString("Nome do componente-chave para buscar (binária): ", alvo, MAX_NOME);
                long cmpBusca = 0;
                clock_t inicio = clock();
                int idx = buscaBinariaPorNome(copia, n, alvo, &cmpBusca);
                clock_t fim = clock();
                double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
                if (idx >= 0) {
                    printf("Componente encontrado em índice %d. Comparações na busca: %ld. Tempo: %.6f s\n",
                           idx, cmpBusca, tempo);
                    printf("-> ");
                    printf("Nome: %s | Tipo: %s | Prioridade: %d\n", copia[idx].nome, copia[idx].tipo, copia[idx].prioridade);
                } else {
                    printf("Componente '%s' não encontrado. Comparações na busca: %ld. Tempo: %.6f s\n", alvo, cmpBusca, tempo);
                }
            }
        } else if (op == 0) break;
        else printf("Opção inválida.\n");
    }
}

/* Menu principal */
int main() {
    printf("=== Sistema de Mochila / Inventário - Níveis Integrados ===\n");
    printf("Instruções: Use os menus para cadastrar, remover, listar e buscar itens.\n");
    printf("As operações mostram contadores de comparações (quando aplicável) e exibem o estado após cada ação.\n\n");
    while (1) {
        printf("\n--- Menu Principal ---\n");
        printf("1) Trabalhar com mochila (vetor)\n");
        printf("2) Trabalhar com mochila (lista encadeada)\n");
        printf("3) Módulo: Componentes da Torre (ordenacões / busca)\n");
        printf("0) Sair\n");
        int op = lerInteiro("Escolha: ");
        if (op == 1) menuVetor();
        else if (op == 2) menuLista();
        else if (op == 3) menuComponentes();
        else if (op == 0) {
            printf("Encerrando. Boa sorte na safe zone!\n");
            /* liberar lista (bom para limpeza) */
            No* ptr = cabecaLista;
            while (ptr) {
                No* tmp = ptr;
                ptr = ptr->proximo;
                free(tmp);
            }
            break;
        } else printf("Opção inválida.\n");
    }
    return 0;
}
