#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define dentroDoTabuleiro (8-l-(sy[j]*i) < 8 && 8-l-(sy[j]*i) >= 0 && c-'a'-(sx[j]*i) < 8 && c-'a'-(sx[j]*i) >= 0)
#define cavaloDentroDoTabuleiro 8-l-py[i] < 8 && 8-l-py[i] >= 0 && c-'a'- px[i] < 8 && c-'a'-px[i] >= 0
#define temPecaBranca (tabuleiro[8-l-(sy[j]*i)][c-'a'-(sx[j]*i)].tipo == 1)
#define temPecaPreta  (tabuleiro[8-l-(sy[j]*i)][c-'a'-(sx[j]*i)].tipo == 2)

typedef struct{
    char nome;
    char mexeu;
    char tipo; //tipo 0 = vazia; tipo 1 = peça branca; tipo 2 = peça preta;
}Peca;

Peca casaBranca = {'#', 0, 0}, casaPreta = {' ', 0, 0};
Peca reiBranco = {'R', 0, 1}, reiPreto = {'r', 0, 2};;
Peca damaBranca = {'D', 0, 1}, damaPreta = {'d', 0, 2};
Peca peaoBranco = {'P', 0, 1}, peaoPreto = {'p', 0, 2};
Peca torreBranca = {'T', 0, 1}, torrePreta = {'t', 0, 2};
Peca bispoBranco = {'B', 0, 1}, bispoPreto = {'b', 0, 2};
Peca cavaloBranco = {'C', 0, 1}, cavaloPreto = {'c', 0, 2};

void duasPecas(char nome, Peca tabuleiro[][8]){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            tabuleiro[i][j] = ((i+j)%2 == 0) ? casaBranca : casaPreta;
        }
    }

    switch (nome)
    {
    case 'p':
        tabuleiro[6][1] = peaoBranco; tabuleiro[1][6] = peaoPreto; break;
    case 't':
        tabuleiro[6][1] = torreBranca; tabuleiro[1][6] = torrePreta; break;
    case 'c':
        tabuleiro[6][1] = cavaloBranco; tabuleiro[1][6] = cavaloPreto; break;
    case 'b':
        tabuleiro[6][1] = bispoBranco; tabuleiro[1][6] = bispoPreto; break;
    case 'd':
        tabuleiro[6][1] = damaBranca; tabuleiro[1][6] = damaPreta; break;
    case 'r':
        tabuleiro[6][1] = reiBranco; tabuleiro[1][6] = reiPreto; break;
    default:
        break;
    }
}

void resetaTabuleiro(Peca tabuleiro[][8]){
    tabuleiro[0][0] = torrePreta; tabuleiro[0][7] = torrePreta; tabuleiro[7][0] = torreBranca; tabuleiro[7][7] = torreBranca;
    tabuleiro[0][1] = cavaloPreto; tabuleiro[0][6] = cavaloPreto; tabuleiro[7][1] = cavaloBranco; tabuleiro[7][6] = cavaloBranco;
    tabuleiro[0][2] = bispoPreto; tabuleiro[0][5] = bispoPreto; tabuleiro[7][2] = bispoBranco; tabuleiro[7][5] = bispoBranco;
    tabuleiro[0][3] = damaPreta; tabuleiro[0][4] = reiPreto; tabuleiro[7][3] = damaBranca; tabuleiro[7][4] = reiBranco;

    for (int i = 0; i < 8; i++) tabuleiro[1][i] = peaoPreto;
    for (int i = 0; i < 8; i++) tabuleiro[6][i] = peaoBranco;

    for (int i = 2; i < 6; i++){
        for (int j = 0; j < 8; j++){
            tabuleiro[i][j] = ((i+j)%2 == 0) ? casaBranca : casaPreta;
        }
    }
}

void printaTabuleiro(Peca tabuleiro[][8]){
    printf("   ___ ___ ___ ___ ___ ___ ___ ___\n");
    for (int i = 0; i < 8; i++){
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; j++){
            printf("| %c ", tabuleiro[i][j].nome);
        }
        printf("|\n");
        printf("  |___|___|___|___|___|___|___|___|\n");
    }
    printf("    a   b   c   d   e   f   g   h  \n");
}