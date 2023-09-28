#include <stdio.h>
#include "movePeca.h"

enum Casa {
    vazia,
    pecaBranca,
    pecaPreta
};

int main(){
    
    FILE *lances;
    Peca tabuleiro[8][8];
    char terminou = 0;
    char tipo, c0, c1;
    int l0, l1;
    int i;

    // duasPecas('t', tabuleiro);
    resetaTabuleiro(tabuleiro);

    lances = fopen("lances.txt", "w");
    if (lances == NULL){ printf("Erro ao abrir o arquivo\n"); free(lances); exit(1); }
    else{ printf("Arquivo aberto\n"); }

    printf("Digite os comandos no formato (tipo da peca) (coordenada inicial) (coordenada final)\n");
    printf("Digite \'X\' para cancelar\n");
    printf("Ex: P e2 e4\n");

    printaTabuleiro(tabuleiro);

    for (i = 0; !terminou; i++){
        // printf("A casa d4 %s em xeque\n", (emXeque(tabuleiro, 0)) ? "esta" : "nao esta");

        if (i%2 == 0){ printf("%d. ", i/2 + 1); scanf(" %c", &tipo); }
        else{ printf("... "); scanf(" %c", &tipo); }

        if (tipo == 'X'){ terminou = 1; break; }
        else if (tipo == 'K'){
            resetaTabuleiro(tabuleiro); i = 0;

            printf("Jogo recomecado!\n");
            printf("Digite os comandos no formato (tipo da peca) (coordenada inicial) (coordenada final)\n");
            printf("Digite \'X\' para cancelar\n");
            printf("Ex: P e2 e4\n");

            printaTabuleiro(tabuleiro);
        }
        scanf(" %c%d %c%d", &c0, &l0, &c1, &l1);

        
        //toLower basicão
        tipo += (tipo <= 'Z') ? 32 : 0;
        c0 += (c0 <= 'Z') ? 32 : 0 ;
        c1 += (c1 <= 'Z') ? 32 : 0 ;

        if (c0 < 'a' || c0 > 'h' || c1 < 'a' || c1 > 'h' || l0 < 1 || l0 > 8 || l1 < 1 || l1 > 8){
            printf("Essas coordenadas nao estao no tabuleiro, tente novamente\n");
            i -= 1;
        }
        else if (tipo == 'p') movePeao(tabuleiro, &i, c0, l0, c1, l1);
        else if (tipo == 't') moveTorre(tabuleiro, &i, c0, l0, c1, l1);
        else if (tipo == 'c') moveCavalo(tabuleiro, &i, c0, l0, c1, l1);
        else if (tipo == 'b') moveBispo(tabuleiro, &i, c0, l0, c1, l1);
        else if (tipo == 'd') moveDama(tabuleiro, &i, c0, l0, c1, l1);
        else if (tipo == 'r') moveRei(tabuleiro, &i, c0, l0, c1, l1);
        else{ printf("Essa nao e uma peca, tente novamente\n"); i--; }

        printaTabuleiro(tabuleiro);
    }

    return 0;
}