#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM 5       // Tamanho da fila circular
#define PILHA_MAX 3 // Tamanho da pilha de reserva
#define MAX_DESFAZER 50 // Histórico de jogadas para desfazer

// Estrutura da peça
typedef struct {
    char nome;  // Tipo da peça: I, O, T, L
    int id;     // Identificador único
} Peca;

// Tipos de jogada para desfazer
typedef enum {JOGAR, RESERVAR, USAR, TROCAR} TipoJogada;

typedef struct {
    TipoJogada tipo;
    Peca p_fila;
    Peca p_pilha;
} Jogada;

// Fila circular
Peca fila[TAM];
int frente = 0;
int tras = 0;
int contador = 0;

// Pilha de reserva
Peca pilha[PILHA_MAX];
int topo = 0;

// Histórico de jogadas
Jogada historico[MAX_DESFAZER];
int historico_idx = 0;

// Função para gerar peça aleatória
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I','O','T','L'};
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Funções da fila
void enqueue(Peca p) {
    if(contador == TAM) return;
    fila[tras] = p;
    tras = (tras + 1) % TAM;
    contador++;
}

Peca dequeue() {
    Peca vazia = {'X', -1};
    if(contador == 0) return vazia;
    Peca removida = fila[frente];
    frente = (frente + 1) % TAM;
    contador--;
    return removida;
}

void mostrarFila() {
    printf("Fila atual: ");
    for(int i = 0; i < contador; i++) {
        int idx = (frente + i) % TAM;
        printf("[%c%d] ", fila[idx].nome, fila[idx].id);
    }
    printf("\n");
}

// Funções da pilha
void push(Peca p) {
    if(topo == PILHA_MAX) return;
    pilha[topo++] = p;
}

Peca pop() {
    Peca vazia = {'X', -1};
    if(topo == 0) return vazia;
    return pilha[--topo];
}

void mostrarPilha() {
    printf("Pilha de reserva: ");
    for(int i = 0; i < topo; i++)
        printf("[%c%d] ", pilha[i].nome, pilha[i].id);
    printf("\n");
}

// Salvar jogada para desfazer
void salvarJogada(TipoJogada tipo, Peca f, Peca p) {
    if(historico_idx < MAX_DESFAZER) {
        historico[historico_idx].tipo = tipo;
        historico[historico_idx].p_fila = f;
        historico[historico_idx].p_pilha = p;
        historico_idx++;
    }
}

// Desfazer última jogada
void desfazer() {
    if(historico_idx == 0) {
        printf("Nada para desfazer!\n");
        return;
    }
    Jogada ultima = historico[--historico_idx];
    switch(ultima.tipo) {
        case JOGAR:
            // Reinsere a peça jogada na frente da fila
            frente = (frente - 1 + TAM) % TAM;
            fila[frente] = ultima.p_fila;
            contador++;
            printf("Última jogada desfeita: Jogar peça\n");
            break;
        case RESERVAR:
            // Remove da pilha e recoloca na frente da fila
            topo--;
            frente = (frente - 1 + TAM) % TAM;
            fila[frente] = ultima.p_fila;
            contador++;
            printf("Última jogada desfeita: Reservar peça\n");
            break;
        case USAR:
            // Reinsere a peça usada de volta na pilha
            pilha[topo++] = ultima.p_pilha;
            printf("Última jogada desfeita: Usar peça reservada\n");
            break;
        case TROCAR:
            // Reverte a troca
            fila[frente] = ultima.p_fila;
            pilha[topo-1] = ultima.p_pilha;
            printf("Última jogada desfeita: Trocar peça\n");
            break;
    }
}

// Trocar topo da pilha com frente da fila
void trocar() {
    if(topo == 0 || contador == 0) {
        printf("Não é possível trocar!\n");
        return;
    }
    Peca temp_fila = fila[frente];
    Peca temp_pilha = pilha[topo-1];
    fila[frente] = temp_pilha;
    pilha[topo-1] = temp_fila;
    salvarJogada(TROCAR, temp_fila, temp_pilha);
    printf("Topo da pilha trocado com frente da fila!\n");
}

// Inverter fila com pilha
void inverter() {
    int i, j;
    // Salva temporariamente fila
    Peca temp_fila[TAM];
    for(i = 0; i < contador; i++)
        temp_fila[i] = fila[(frente + i) % TAM];
    int tam_fila = contador;

    // Inverte fila com pilha
    contador = topo;
    for(i = 0; i < contador; i++)
        fila[(frente + i) % TAM] = pilha[i];
    topo = tam_fila;
    for(i = 0; i < topo; i++)
        pilha[i] = temp_fila[i];

    printf("Fila e pilha invertidas!\n");
}

int main() {
    srand(time(NULL));
    int id_atual = 1;

    // Inicializa fila com 5 peças
    for(int i = 0; i < TAM; i++)
        enqueue(gerarPeca(id_atual++));

    int opcao;

    do {
        printf("\n=== Menu Tetris Stack Mestre ===\n");
        printf("1- Jogar peça\n");
        printf("2- Reservar peça\n");
        printf("3- Usar peça reservada\n");
        printf("4- Trocar topo da pilha com frente da fila\n");
        printf("5- Desfazer última jogada\n");
        printf("6- Inverter fila com pilha\n");
        printf("0- Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: {
                Peca jogada = dequeue();
                if(jogada.id != -1) {
                    printf("Você jogou: %c%d\n", jogada.nome, jogada.id);
                    salvarJogada(JOGAR, jogada, jogada);
                    enqueue(gerarPeca(id_atual++));
                }
                break;
            }
            case 2: {
                Peca reservada = dequeue();
                if(reservada.id != -1) {
                    push(reservada);
                    printf("Você reservou: %c%d\n", reservada.nome, reservada.id);
                    salvarJogada(RESERVAR, reservada, reservada);
                    enqueue(gerarPeca(id_atual++));
                }
                break;
            }
            case 3: {
                Peca usada = pop();
                if(usada.id != -1) {
                    printf("Você usou a peça reservada: %c%d\n", usada.nome, usada.id);
                    salvarJogada(USAR, usada, usada);
                }
                break;
            }
            case 4:
                trocar();
                break;
            case 5:
                desfazer();
                break;
            case 6:
                inverter();
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

        mostrarFila();
        mostrarPilha();

    } while(opcao != 0);

    return 0;
}
