#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define NUM_TERRITORIOS 5
#define NUM_MISSOES 3
#define MAX_NOME 30
#define MAX_COR 10

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR]; // cor do dono (Jogador, Inimigo, Neutro)
    int tropas;
} Territorio;

// === Protótipos ===
Territorio* alocarMapa(int);
void inicializarTerritorios(Territorio*, int);
void liberarMemoria(Territorio*);
void exibirMenuPrincipal();
void exibirMapa(const Territorio*, int);
void exibirMissao(int);
void faseDeAtaque(Territorio*, int);
void simularAtaque(Territorio*, int, int);
int sortearMissao();
int verificarVitoria(const Territorio*, int, int);
void limparBufferEntrada();

// === Função principal ===
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    Territorio* mapa = alocarMapa(NUM_TERRITORIOS);
    if (!mapa) {
        printf("Erro ao alocar mapa.\n");
        return 1;
    }

    inicializarTerritorios(mapa, NUM_TERRITORIOS);

    int idMissao = sortearMissao();
    int opcao;

    do {
        exibirMapa(mapa, NUM_TERRITORIOS);
        exibirMissao(idMissao);
        exibirMenuPrincipal();

        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, NUM_TERRITORIOS);
                break;
            case 2:
                if (verificarVitoria(mapa, NUM_TERRITORIOS, idMissao)) {
                    printf("\n🎉 Missão cumprida! Você venceu o jogo!\n");
                    opcao = 0;
                } else {
                    printf("\n❌ Missão ainda não cumprida. Continue lutando!\n");
                }
                break;
            case 0:
                printf("\nSaindo do jogo...\n");
                break;
            default:
                printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);

    liberarMemoria(mapa);
    return 0;
}

// === Implementações ===

Territorio* alocarMapa(int n) {
    return (Territorio*) calloc(n, sizeof(Territorio));
}

void inicializarTerritorios(Territorio* mapa, int n) {
    const char* nomes[] = {"Acre", "Bahia", "Ceará", "São Paulo", "Amazonas"};
    for (int i = 0; i < n; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        strcpy(mapa[i].cor, (i < 2) ? "Jogador" : "Inimigo");  // primeiros 2 para jogador
        mapa[i].tropas = 3 + rand() % 4; // 3 a 6 tropas
    }
}

void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

void exibirMenuPrincipal() {
    printf("\n===== MENU =====\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missão\n");
    printf("0 - Sair\n");
}

void exibirMapa(const Territorio* mapa, int n) {
    printf("\n----- MAPA -----\n");
    for (int i = 0; i < n; i++) {
        printf("%d. %s | Tropas: %d | Dono: %s\n", i + 1, mapa[i].nome, mapa[i].tropas, mapa[i].cor);
    }
    printf("----------------\n");
}

void exibirMissao(int id) {
    printf("\n🎯 Missão atual: ");
    switch (id) {
        case 1:
            printf("Conquistar 3 territórios.\n");
            break;
        case 2:
            printf("Eliminar todos os territórios inimigos.\n");
            break;
        case 3:
            printf("Conquistar o território 'Amazonas'.\n");
            break;
    }
}

void faseDeAtaque(Territorio* mapa, int n) {
    int at, def;
    printf("\nDigite o número do território ATACANTE (1 a %d): ", n);
    scanf("%d", &at);
    printf("Digite o número do território DEFENSOR (1 a %d): ", n);
    scanf("%d", &def);
    limparBufferEntrada();

    if (at < 1 || at > n || def < 1 || def > n || at == def) {
        printf("\n❌ Territórios inválidos.\n");
        return;
    }

    if (strcmp(mapa[at - 1].cor, "Jogador") != 0) {
        printf("\n⚠️ Você só pode atacar a partir de seus próprios territórios.\n");
        return;
    }

    simularAtaque(mapa, at - 1, def - 1);
}

void simularAtaque(Territorio* mapa, int atacante, int defensor) {
    if (mapa[atacante].tropas < 1) {
        printf("\n⚠️ Tropas insuficientes para atacar.\n");
        return;
    }

    int dadoAtq = rand() % 6 + 1;
    int dadoDef = rand() % 6 + 1;

    printf("\n🎲 Ataque: %d vs Defesa: %d\n", dadoAtq, dadoDef);

    if (dadoAtq >= dadoDef) {
        printf("✅ Ataque bem-sucedido!\n");
        mapa[defensor].tropas--;
        if (mapa[defensor].tropas <= 0) {
            printf("🎉 Território conquistado!\n");
            strcpy(mapa[defensor].cor, "Jogador");
            mapa[defensor].tropas = 1;
            mapa[atacante].tropas--; // Move uma tropa
        }
    } else {
        printf("❌ Defesa resistiu. Você perdeu uma tropa.\n");
        mapa[atacante].tropas--;
    }
}

int sortearMissao() {
    return 1 + rand() % NUM_MISSOES;
}

int verificarVitoria(const Territorio* mapa, int n, int idMissao) {
    int conquistados = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(mapa[i].cor, "Jogador") == 0) {
            conquistados++;
            if (idMissao == 3 && strcmp(mapa[i].nome, "Amazonas") == 0)
                return 1;
        }
    }

    switch (idMissao) {
        case 1:
            return conquistados >= 3;
        case 2:
            return conquistados == n;
    }

    return 0;
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
