// ============================================================================
//           PROJETO TETRIS STACK - DESAFIO DE CÓDIGO (NÍVEL MESTRE)
// ============================================================================
//
// OBJETIVOS (VERSÃO FINAL):
// - Gerenciar peças com fila circular (peças futuras) e pilha (reservadas).
// - Permitir jogar, reservar, usar e realizar trocas entre as estruturas.
// - Implementar trocas pontuais (1x1) e múltiplas (3x3).
// - A fila deve permanecer sempre cheia; a pilha tem capacidade limitada.
// - Peças são geradas automaticamente com tipo aleatório e ID sequencial.
//
// ============================================================================
// AUTOR: Diego Bloise
// DATA: Novembro de 2025
// LINGUAGEM: C (ANSI C - padrão C99)
// COMPILAÇÃO (exemplo):
//     gcc tetris.c -o tetris
//
// EXECUÇÃO:
//     ./tetris
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ============================================================================
//                         CONSTANTES E ESTRUTURAS
// ============================================================================

#define CAP_FILA 5   /**< Capacidade máxima da fila circular */
#define CAP_PILHA 3  /**< Capacidade máxima da pilha linear */

/**
 * @struct Peca
 * @brief Representa uma peça do jogo Tetris.
 *
 * Cada peça possui um tipo (I, O, T, L) e um identificador único.
 */
typedef struct {
    char tipo; /**< Tipo da peça (I, O, T, L) */
    int id;    /**< Identificador único */
} Peca;

/**
 * @struct Fila
 * @brief Estrutura que implementa uma fila circular de peças.
 */
typedef struct {
    Peca itens[CAP_FILA]; /**< Vetor de peças armazenadas */
    int frente;           /**< Índice da frente */
    int tras;             /**< Índice do final */
    int quantidade;       /**< Quantidade atual de elementos */
} Fila;

/**
 * @struct Pilha
 * @brief Estrutura que representa uma pilha linear de peças.
 */
typedef struct {
    Peca itens[CAP_PILHA]; /**< Vetor de peças armazenadas */
    int topo;              /**< Índice do topo da pilha */
} Pilha;

// ============================================================================
//                             FUNÇÕES DA FILA
// ============================================================================

/**
 * @brief Inicializa a fila circular.
 */
void inicializarFila(Fila* fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->quantidade = 0;
}

/**
 * @brief Verifica se a fila está cheia.
 * @return 1 se cheia, 0 caso contrário.
 */
int filaCheia(Fila* fila) {
    return fila->quantidade == CAP_FILA;
}

/**
 * @brief Verifica se a fila está vazia.
 * @return 1 se vazia, 0 caso contrário.
 */
int filaVazia(Fila* fila) {
    return fila->quantidade == 0;
}

/**
 * @brief Insere uma nova peça no final da fila.
 * @param fila Ponteiro para a fila.
 * @param nova Peça a ser inserida.
 */
void enqueue(Fila* fila, Peca nova) {
    if (filaCheia(fila)) return;

    fila->tras = (fila->tras + 1) % CAP_FILA;
    fila->itens[fila->tras] = nova;
    fila->quantidade++;
}

/**
 * @brief Remove a peça da frente da fila.
 * @param fila Ponteiro para a fila.
 * @return Peça removida; caso vazia, retorna tipo '-' e id -1.
 */
Peca dequeue(Fila* fila) {
    Peca removida = { '-', -1 };

    if (filaVazia(fila)) return removida;

    removida = fila->itens[fila->frente];
    fila->frente = (fila->frente + 1) % CAP_FILA;
    fila->quantidade--;

    return removida;
}

/**
 * @brief Retorna a peça da frente da fila sem removê-la.
 * @param fila Ponteiro para a fila.
 * @return Ponteiro para a peça ou NULL se vazia.
 */
Peca* frenteFila(Fila* fila) {
    if (filaVazia(fila)) return NULL;
    return &fila->itens[fila->frente];
}

/**
 * @brief Exibe todas as peças presentes na fila circular.
 */
void mostrarFila(Fila* fila) {
    printf("\nFila de Peças Futuras:\t");

    if (filaVazia(fila)) {
        printf("[vazia]");
        return;
    }

    int index = fila->frente;
    for (int i = 0; i < fila->quantidade; i++) {
        Peca p = fila->itens[index];
        printf("[%c %d] ", p.tipo, p.id);
        index = (index + 1) % CAP_FILA;
    }
    printf("\n");
}

// ============================================================================
//                             FUNÇÕES DA PILHA
// ============================================================================

/**
 * @brief Inicializa a pilha de peças reservadas.
 */
void inicializarPilha(Pilha* pilha) {
    pilha->topo = -1;
}

/**
 * @brief Verifica se a pilha está cheia.
 * @return 1 se cheia, 0 caso contrário.
 */
int pilhaCheia(Pilha* pilha) {
    return pilha->topo == CAP_PILHA - 1;
}

/**
 * @brief Verifica se a pilha está vazia.
 * @return 1 se vazia, 0 caso contrário.
 */
int pilhaVazia(Pilha* pilha) {
    return pilha->topo == -1;
}

/**
 * @brief Adiciona uma nova peça ao topo da pilha.
 * @param pilha Ponteiro para a pilha.
 * @param nova Peça a ser inserida.
 */
void push(Pilha* pilha, Peca nova) {
    if (pilhaCheia(pilha)) {
        printf("A pilha está cheia! Não é possível reservar mais peças.\n");
        return;
    }
    pilha->itens[++pilha->topo] = nova;
}

/**
 * @brief Remove a peça do topo da pilha.
 * @param pilha Ponteiro para a pilha.
 * @return Peça removida; caso vazia, retorna tipo '-' e id -1.
 */
Peca pop(Pilha* pilha) {
    Peca removida = { '-', -1 };
    if (pilhaVazia(pilha)) {
        printf("A pilha está vazia! Nenhuma peça reservada.\n");
        return removida;
    }
    return pilha->itens[pilha->topo--];
}

/**
 * @brief Retorna a peça do topo sem removê-la.
 * @param pilha Ponteiro para a pilha.
 * @return Ponteiro para a peça do topo ou NULL se vazia.
 */
Peca* topoPilha(Pilha* pilha) {
    if (pilhaVazia(pilha)) return NULL;
    return &pilha->itens[pilha->topo];
}

/**
 * @brief Exibe o estado atual da pilha (do topo à base).
 */
void mostrarPilha(Pilha* pilha) {
    printf("Pilha de Reservas (Topo → Base): ");

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
//                           FUNÇÕES DE UTILIDADE
// ============================================================================

/**
 * @brief Gera uma nova peça aleatória.
 * @param id Identificador único a ser atribuído à peça.
 * @return Estrutura Peca preenchida.
 */
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = { 'I', 'O', 'T', 'L' };
    nova.tipo = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

/**
 * @brief Exibe o estado atual da fila e da pilha.
 */
void mostrarEstado(Fila* fila, Pilha* pilha) {
    printf("\n============================================\n");
    mostrarFila(fila);
    mostrarPilha(pilha);
    printf("\n============================================\n");
}

/**
 * @brief Troca a peça da frente da fila com o topo da pilha.
 */
void trocarPecaAtual(Fila* fila, Pilha* pilha) {
    if (filaVazia(fila) || pilhaVazia(pilha)) {
        printf("Não é possível realizar a troca (fila ou pilha vazia).\n");
        return;
    }

    Peca* frente = frenteFila(fila);
    Peca* topo = topoPilha(pilha);

    Peca temp = *frente;
    *frente = *topo;
    *topo = temp;

    printf("Troca entre a frente da fila e o topo da pilha realizada!\n");
}

/**
 * @brief Troca múltipla entre as 3 primeiras peças da fila e as 3 do topo da pilha.
 */
void trocaMultipla(Fila* fila, Pilha* pilha) {
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

    printf("Troca múltipla (3x3) concluída com sucesso!\n");
}

// ============================================================================
//                              FUNÇÃO PRINCIPAL
// ============================================================================

/**
 * @brief Função principal do jogo Tetris Stack.
 *
 * Controla o fluxo de interação do usuário, incluindo geração de peças,
 * manipulação das estruturas e execução das trocas.
 *
 * @return int Código de status (0 = sucesso).
 */
int main() {
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    srand(time(NULL));

    int opcao;
    int idAtual = 0;

    // Inicializa a fila com peças aleatórias
    for (int i = 0; i < CAP_FILA; i++) {
        enqueue(&fila, gerarPeca(idAtual++));
    }

    // Loop principal de ações
    do {
        mostrarEstado(&fila, &pilha);

        printf("\n----------- MENU DE AÇÕES -----------\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Reservar peça (enviar para pilha)\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila ↔ topo da pilha\n");
        printf("5 - Troca múltipla (3 primeiras ↔ 3 da pilha)\n");
        printf("0 - Sair\n");
        printf("------------------------------------\n");
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

            case 3: { // Usar peça reservada
                Peca usada = pop(&pilha);
                if (usada.id != -1) {
                    printf("Você usou a peça [%c %d]\n", usada.tipo, usada.id);
                }
                break;
            }

            case 4:
                trocarPecaAtual(&fila, &pilha);
                break;

            case 5:
                trocaMultipla(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando o jogo Tetris Stack...\n");
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    printf("\nJogo finalizado. Obrigado por jogar!\n");
    return 0;
}
