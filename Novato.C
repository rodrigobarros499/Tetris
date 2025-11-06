#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM 5  // Tamanho da fila circular

// Estrutura da peça
typedef struct {
    char nome;  // Tipo da peça: I, O, T, L
    int id;     // Identificador único
} Peca;

// Fila circular
Peca fila[TAM];
int frente = 0;     // Índice da peça que será jogada
int tras = 0;       // Índice para inserir a próxima peça
int contador = 0;   // Quantidade de peças atualmente na fila

// Função para gerar uma peça aleatória
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I','O','T','L'};
    nova.nome = tipos[rand() % 4];  // Escolhe aleatoriamente um tipo
    nova.id = id;
    return nova;
}

// Inserir peça na fila (enqueue)
void enqueue(Peca p) {
    if(contador == TAM) {
        printf("Fila cheia!\n");
        return;
    }
    fila[tras] = p;
    tras = (tras + 1) % TAM;  // Mantém a circularidade
    contador++;
}

// Remover peça da fila (dequeue)
Peca dequeue() {
    Peca vazia = {'X', -1}; // Retorno caso a fila esteja vazia
    if(contador == 0) {
        printf("Fila vazia!\n");
        return vazia;
    }
    Peca removida = fila[frente];
    frente = (frente + 1) % TAM;
    contador--;
    return removida;
}

// Mostrar fila atual
void mostrarFila() {
    printf("Fila atual: ");
    for(int i = 0; i < contador; i++) {
        int idx = (frente + i) % TAM;
        printf("[%c%d] ", fila[idx].nome, fila[idx].id);
    }
    printf("\n");
}

// Função principal
int main() {
    srand(time(NULL));  // Inicializa gerador de números aleatórios
    int id_atual = 1;

    // Inicializa fila com 5 peças
    for(int i = 0; i < TAM; i++) {
        enqueue(gerarPeca(id_atual++));
    }

    int opcao;

    do {
        printf("\n=== Menu Tetris Stack ===\n");
        printf("1- Jogar peça\n");
        printf("2- Inserir nova peça\n");
        printf("3- Mostrar fila\n");
        printf("0- Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: {
                Peca jogada = dequeue();
                if(jogada.id != -1)
                    printf("Você jogou: %c%d\n", jogada.nome, jogada.id);
                break;
            }
            case 2:
                enqueue(gerarPeca(id_atual++));
                printf("Nova peça inserida!\n");
                break;
            case 3:
                mostrarFila();
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while(opcao != 0);

    return 0;
}
