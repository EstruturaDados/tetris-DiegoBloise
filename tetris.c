#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ============================================================================
//             DESAFIO TETRIS STACK - NÍVEL MESTRE
// ============================================================================
//
// OBJETIVOS:
// - Gerenciar peças com fila circular (futuras) e pilha (reservadas).
// - Permitir jogar, reservar, usar e realizar trocas entre as estruturas.
// - Troca individual: frente da fila <-> topo da pilha.
// - Troca múltipla: 3 primeiras da fila <-> 3 peças da pilha.
// - A fila permanece cheia; a pilha tem capacidade limitada.
// - Peças geradas automaticamente com tipo aleatório e id sequencial.
//
// ============================================================================

// Capacidades fixas
#define CAP_FILA 5
#define CAP_PILHA 3

// Estrutura que representa uma peça do Tetris
typedef struct {
    char tipo; // Representa a forma da peça: I, O, T, L
    int id;    // Identificador único da peça
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[CAP_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// Estrutura da pilha linear
typedef struct {
    Peca itens[CAP_PILHA];
    int topo;
} Pilha;

// ============================================================================
//                           FUNÇÕES DA FILA
// ============================================================================

// Inicializa a fila
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->quantidade = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *fila) {
    return fila->quantidade == CAP_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *fila) {
    return fila->quantidade == 0;
}

// Insere peça na fila (enqueue)
void enqueue(Fila *fila, Peca nova) {
    if (filaCheia(fila)) return;

    fila->tras = (fila->tras + 1) % CAP_FILA;
    fila->itens[fila->tras] = nova;
    fila->quantidade++;
}

// Remove peça da frente (dequeue)
Peca dequeue(Fila *fila) {
    Peca removida = { '-', -1 };

    if (filaVazia(fila)) return removida;

    removida = fila->itens[fila->frente];
    fila->frente = (fila->frente + 1) % CAP_FILA;
    fila->quantidade--;

    return removida;
}

// Acessa peça da frente (sem remover)
Peca *frenteFila(Fila *fila) {
    if (filaVazia(fila)) return NULL;
    return &fila->itens[fila->frente];
}

// Exibe a fila de peças futuras
void mostrarFila(Fila *fila) {
    printf("\nFila de Peças:\t");

    if (filaVazia(fila)) {
        printf("[vazia]");
        return;
    }

    int i, index = fila->frente;
    for (i = 0; i < fila->quantidade; i++) {
        Peca p = fila->itens[index];
        printf("[%c %d] ", p.tipo, p.id);
        index = (index + 1) % CAP_FILA;
    }
    printf("\n");
}

// ============================================================================
//                           FUNÇÕES DA PILHA
// ============================================================================

// Inicializa a pilha
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *pilha) {
    return pilha->topo == CAP_PILHA - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

// Insere peça no topo (push)
void push(Pilha *pilha, Peca nova) {
    if (pilhaCheia(pilha)) {
        printf("A pilha está cheia! Não é possível reservar mais peças.\n");
        return;
    }
    pilha->itens[++pilha->topo] = nova;
}

// Remove peça do topo (pop)
Peca pop(Pilha *pilha) {
    Peca removida = { '-', -1 };
    if (pilhaVazia(pilha)) {
        printf("A pilha está vazia! Nenhuma peça reservada para usar.\n");
        return removida;
    }
    return pilha->itens[pilha->topo--];
}

// Acessa peça do topo (sem remover)
Peca *topoPilha(Pilha *pilha) {
    if (pilhaVazia(pilha)) return NULL;
    return &pilha->itens[pilha->topo];
}

// Exibe o estado da pilha
void mostrarPilha(Pilha *pilha) {
    printf("Pilha de Reserva (Topo -> Base):\t");

    if (pilhaVazia(pilha)) {
        printf("[vazia]");
        return;
    }

    for (int i = pilha->topo; i >= 0; i--) {
        Peca p = pilha->itens[i];
        printf("[%c %d] ", p.tipo, p.id);
    }
    printf("\n");
}

// ============================================================================
//                       FUNÇÕES DE UTILIDADE GERAL
// ============================================================================

// Gera uma nova peça aleatória
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = { 'I', 'O', 'T', 'L' };
    nova.tipo = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Exibe fila e pilha juntas
void mostrarEstado(Fila *fila, Pilha *pilha) {
    printf("\n============================================\n");
    mostrarFila(fila);
    mostrarPilha(pilha);
    printf("============================================\n");
}

// Troca a peça da frente da fila com o topo da pilha
void trocarPecaAtual(Fila *fila, Pilha *pilha) {
    if (filaVazia(fila) || pilhaVazia(pilha)) {
        printf("Não é possível realizar a troca (fila ou pilha vazia).\n");
        return;
    }

    Peca *frente = frenteFila(fila);
    Peca *topo = topoPilha(pilha);

    Peca temp = *frente;
    *frente = *topo;
    *topo = temp;

    printf("Troca entre a peça da frente da fila e o topo da pilha realizada com sucesso.\n");
}

// Troca as 3 primeiras da fila com as 3 da pilha
void trocaMultipla(Fila *fila, Pilha *pilha) {
    if (fila->quantidade < 3 || pilha->topo < 2) {
        printf("Não há peças suficientes para a troca múltipla.\n");
        return;
    }

    int indexFila = fila->frente;
    for (int i = 0; i < 3; i++) {
        Peca temp = fila->itens[(indexFila + i) % CAP_FILA];
        fila->itens[(indexFila + i) % CAP_FILA] = pilha->itens[pilha->topo - i];
        pilha->itens[pilha->topo - i] = temp;
    }

    printf("Troca múltipla entre as 3 primeiras da fila e as 3 da pilha concluída.\n");
}

// ============================================================================
//                              FUNÇÃO PRINCIPAL
// ============================================================================

int main() {
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    srand(time(NULL));

    int opcao;
    int idAtual = 0;

    // Inicializa a fila com 5 peças
    for (int i = 0; i < CAP_FILA; i++) {
        enqueue(&fila, gerarPeca(idAtual++));
    }

    do {
        mostrarEstado(&fila, &pilha);

        printf("\nMenu de Ações:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Reservar peça (enviar para pilha)\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: { // Jogar peça
                Peca jogada = dequeue(&fila);
                if (jogada.id != -1) {
                    printf("Você jogou a peça [%c %d]\n", jogada.tipo, jogada.id);
                    enqueue(&fila, gerarPeca(idAtual++));
                }
                break;
            }

            case 2: { // Reservar peça
                if (pilhaCheia(&pilha)) {
                    printf("A pilha está cheia! Libere espaço antes de reservar mais.\n");
                    break;
                }
                Peca reservada = dequeue(&fila);
                if (reservada.id != -1) {
                    printf("Peça [%c %d] movida para a pilha de reserva.\n", reservada.tipo, reservada.id);
                    push(&pilha, reservada);
                    enqueue(&fila, gerarPeca(idAtual++));
                }
                break;
            }

            case 3: { // Usar peça da reserva
                Peca usada = pop(&pilha);
                if (usada.id != -1) {
                    printf("Você usou a peça [%c %d] da reserva.\n", usada.tipo, usada.id);
                    enqueue(&fila, gerarPeca(idAtual++));
                }
                break;
            }

            case 4: // Troca simples
                trocarPecaAtual(&fila, &pilha);
                break;

            case 5: // Troca múltipla
                trocaMultipla(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando o programa...\n");
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
