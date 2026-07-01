// mostrar na tela
#include "funcao.h"

void criar_borda_jogo() {

    
    Clic_move(1,1);
    Clic_printBox(largura,altura);
    Clic_move(2,2);
}

void limpar_tela() {
    system("clear");
    criar_borda_jogo();
}

void logo(){
    int sla = (strlen("impossible edition ")/2) ;
    
    Clic_move(4, 3); // Posiciona dentro da borda

    printf("██████╗  ██╗              ████████╗██╗  ██╗███████╗     ██████╗  █████╗ ███╗   ███╗███████╗\n");
    Clic_moveToColumn(3);
    printf("╚════██╗███║              ╚══██╔══╝██║  ██║██╔════╝    ██╔════╝ ██╔══██╗████╗ ████║██╔════╝\n");
    Clic_moveToColumn(3);
    printf(" █████╔╝╚██║    █████╗       ██║   ███████║█████╗      ██║  ███╗███████║██╔████╔██║█████╗  \n");
    Clic_moveToColumn(3);
    printf("██╔═══╝  ██║    ╚════╝       ██║   ██╔══██║██╔══╝      ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝  \n");
    Clic_moveToColumn(3);
    printf("███████╗ ██║                 ██║   ██║  ██║███████╗    ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗\n");
    Clic_moveToColumn(3);
    printf("╚══════╝ ╚═╝                 ╚═╝   ╚═╝  ╚═╝╚══════╝     ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝\n");
    Clic_moveToColumn(3);
    Clic_moveToColumn((92/2)- sla); 
    printf("-----impossible edition-----\n");
    Clic_moveToColumn(3);
    printf("\n");
}

void mostrar_menu(int opcao_selecionada) {
    limpar_tela();
    
    // Título do jogo
    logo();
    
    // Opção 1: Iniciar Jogo
    Clic_moveDown(28);
    if (opcao_selecionada == 0) {
        Clic_moveToColumn(3);
        printf("  ► [INICIAR JOGO]\n");
    } else {
        Clic_moveToColumn(3);
        printf("    INICIAR JOGO\n");
    }
    
    // Opção 2: Ranking
    if (opcao_selecionada == 1) {
        Clic_moveToColumn(3);
        printf("  ► [RANKING DE JOGADORES]\n");
    } else {
        Clic_moveToColumn(3);
        printf("    RANKING DE JOGADORES\n");
    }
    
    // Opção 3: Sair
    if (opcao_selecionada == 2) {
        Clic_moveToColumn(3);
        printf("  ► [SAIR]\n");
    } else {
        Clic_moveToColumn(3);
        printf("    SAIR\n");
    }
    Clic_breakLine();
    Clic_moveToColumn(3);
    Clic_printBox(largura-4, 3);
    Clic_moveUpBegin(1);
    Clic_moveToColumn(Clic_getScreenCenterColumn()-(42/2));
    printf(" ↑↓ : Navegar  │  ENTER: Selecionar");
    Clic_fixDraw();
}

void placa (int tentativa, int meta, int soma_total){
    Clic_printBox(largura-4,3);
    Clic_move(3,3);

        Clic_moveToColumn(Clic_getScreenCenterColumn()-34);
    printf("  Meta: %d pontos | Sua pontuação: %d|  Tentativas restantes: %d\n", meta, soma_total, tentativa);
}

void m_carta(struct carta baralho[MAX_B], int *i) {

    Clic_printBox(9, 7);
    Clic_moveUp(5);
    Clic_moveLeft(8);
    switch (baralho[*i].naipe) {
        case 1: printf("♥"); break;
        case 2: printf("♣"); break;
        case 3: printf("♦"); break;
        case 4: printf("♠"); break;
    }
    Clic_moveDown(4);
    Clic_moveRight(3);
    
    printf("%2d", baralho[*i].numero);
    Clic_moveUp(5);
    Clic_moveRight(3);

}

void m_mao(struct carta baralho[MAX_B], int i, int sua_m[MAX_M]) {
    // Linha superior das cartas
    for (int j = 0; j < MAX_M; j++) { 
        if (sua_m[j] != -1) {
            m_carta(baralho, &sua_m[j]);
            Clic_moveRight(3);
        }
    }
}

// destaca da carta selecionada
void m_mao_selecionavel(struct carta baralho[MAX_B], int sua_m[MAX_M], int selecionado) {
    // Linha do meio com naipe e número
    for (int j = 0; j < MAX_M; j++) {
        if (sua_m[j] != -1) {
            m_carta(baralho, &sua_m[j]);
        }
    }
    Clic_moveDownBegin(7);

    Clic_moveToColumn(Clic_getScreenCenterColumn()-30);
    // Numeração das posições
    for (int j = 0; j < MAX_M; j++) {
        if (sua_m[j] != -1) {
            if (j == selecionado) {
                printf("    [%d]   ", j + 1);
            } else {
                printf("     %d    ", j + 1);
            }
        } else {
            printf("        ");
        }
    }
    printf("\n");
}

// mostra a mensagem final
void mensagem_f(int soma_total_final, int meta) {

    // criar caixa
    printf("╔═══════════════════════════════════════╗\n");
    printf("║         RESULTADO FINAL               ║\n");
    printf("╚═══════════════════════════════════════╝\n\n");
    

    if (soma_total_final < meta) {
        printf("Foi por pouco! Faltaram %d pontos\n", meta - soma_total_final);
    } else if (soma_total_final == meta) {
        printf("NA MOSCA! Parabéns!\n");
    } else {
        printf("se salvou mas perdeu! Excedeu por %d pontos\n", soma_total_final - meta);
    }
}
