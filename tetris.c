#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ============================================================================
//             DESAFIO TETRIS STACK - NÍVEL NOVATO
// ============================================================================
//
// OBJETIVOS:
// - Implementar uma fila circular que simula as próximas peças do Tetris Stack.
// - Inserção (enqueue), remoção (dequeue) e visualização (mostrarFila).
// - As peças são geradas automaticamente com tipo aleatório e id sequencial.
// - Após cada jogada (remoção), uma nova peça é inserida no final da fila.
// ============================================================================

// Capacidade fixa da fila
#define CAPACIDADE 5

// Estrutura que representa uma peça do Tetris
typedef struct {
    char tipo; // Representa a forma da peça: I, O, T, L
    int id;    // Identificador único da peça
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[CAPACIDADE];
    int frente;
    int tras;
    int quantidade;
} Fila;

// ============================================================================
//                        FUNÇÕES DE CONTROLE DA FILA
// ============================================================================

// Inicializa a fila
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->quantidade = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *fila) {
    return fila->quantidade == CAPACIDADE;
}

// Verifica se a fila está vazia
int filaVazia(Fila *fila) {
    return fila->quantidade == 0;
}

// Gera uma nova peça automaticamente
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = { 'I', 'O', 'T', 'L' };
    nova.tipo = tipos[rand() % 4]; // Escolhe tipo aleatório
    nova.id = id; // Define id sequencial
    return nova;
}

// Insere uma peça no final da fila (enqueue)
void enqueue(Fila *fila, Peca nova) {
    if (filaCheia(fila)) {
        printf("A fila está cheia! Não é possível inserir nova peça.\n");
        return;
    }

    fila->tras = (fila->tras + 1) % CAPACIDADE;
    fila->itens[fila->tras] = nova;
    fila->quantidade++;
}

// Remove a peça da frente da fila (dequeue)
Peca dequeue(Fila *fila) {
    Peca removida = { '-', -1 }; // Peça “vazia” para retorno padrão

    if (filaVazia(fila)) {
        printf("A fila está vazia! Nenhuma peça para jogar.\n");
        return removida;
    }

    removida = fila->itens[fila->frente];
    fila->frente = (fila->frente + 1) % CAPACIDADE;
    fila->quantidade--;

    return removida;
}

// Exibe o estado atual da fila
void mostrarFila(Fila *fila) {
    printf("\nFila de Peças Futuras:\n");

    if (filaVazia(fila)) {
        printf("[vazia]\n");
        return;
    }

    int i;
    int index = fila->frente;

    for (i = 0; i < fila->quantidade; i++) {
        Peca p = fila->itens[index];
        printf("[%c %d] ", p.tipo, p.id);
        index = (index + 1) % CAPACIDADE;
    }

    printf("\n");
}

// ============================================================================
//                              FUNÇÃO PRINCIPAL
// ============================================================================

int main() {
    Fila fila;
    inicializarFila(&fila);
    srand(time(NULL));

    int opcao;
    int idAtual = 0;

    // Inicializa a fila com 5 peças
    for (int i = 0; i < CAPACIDADE; i++) {
        enqueue(&fila, gerarPeca(idAtual++));
    }

    do {
        mostrarFila(&fila);

        printf("\nMenu de Ações:\n");
        printf("1 - Jogar peça (remover da frente)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = dequeue(&fila);
                if (jogada.id != -1) {
                    printf("Você jogou a peça [%c %d]\n", jogada.tipo, jogada.id);
                    // Após jogar, adiciona uma nova peça
                    enqueue(&fila, gerarPeca(idAtual++));
                }
                break;
            }
            case 0:
                printf("\nEncerrando o programa...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
