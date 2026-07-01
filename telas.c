#include "funcao.h"

void tela_principal(int tentativa, int meta, int soma_total, struct carta baralho[MAX_B], int i, int sua_m[MAX_M], int *resposta) {
    placa(tentativa, meta, soma_total);

        // mostra a mao do jogador
    Clic_moveDown(27);
    Clic_moveToColumn(3);
    Clic_breakLine();
    printf("sua mão:\n");

    Clic_moveToColumn(Clic_getScreenCenterColumn()-30);
    m_mao(baralho, i, sua_m);
        
    // PERGUNTA SE DESEJA CONTINUAR
    Clic_moveDown(8);
    Clic_moveToColumn(3);
        Clic_moveToColumn(Clic_getScreenCenterColumn() - 26);
    Clic_printBox(52,4);

    Clic_moveUpBegin(4);
    Clic_printCenter("Deseja continuar jogando?\n");
    Clic_moveToColumn(4);
        Clic_moveDown(1);
    Clic_moveToColumn(Clic_getScreenCenterColumn() - 25 );
    printf("1 - Sim, puxar carta | 0 - Naõ, terminar agora\n");
    Clic_moveToColumn(4);
    Clic_printCenter("Escolha: ");

    scanf("%d", resposta);
    getchar();

}

void tela_mostra_carta(int tentativa, int meta, int soma_total, struct carta baralho[MAX_B], int i, int sua_m[MAX_M], int *somar_visivel, int manga[MAX_M], int *soma_oculta, int trapaceiro_mode){

    limpar_tela();
    placa(tentativa, meta, soma_total);

    Clic_moveToColumn(3);
    printf("Carta puxada:\n");
    Clic_move(Clic_getScreenCenterLine()/4, Clic_getScreenCenterColumn()-3);
    m_carta(baralho, &i);

    Clic_moveDown(27);
    Clic_moveToColumn(3);
    printf("sua mão:\n");

    Clic_moveToColumn(Clic_getScreenCenterColumn()-30);
    m_mao(baralho, i, sua_m);
        
        // PERGUNTA SE DESEJA CONTINUAR
    Clic_moveDown(7);
    salva_c(&tentativa, sua_m, i, somar_visivel, baralho, manga, soma_oculta, trapaceiro_mode);  

}

int escolher_posicao_mao(struct carta baralho[MAX_B], int sua_m[MAX_M], int carta_nova, int manga[MAX_M], int *soma_oculta, int trapaceiro_mode) {
    int selecionado = 0;
    int key;
    int mostrar_oculta = 0;
    
    // Encontra a primeira posição válida
    while (selecionado < MAX_M && sua_m[selecionado] == -1) {
        selecionado++;
    }
    
    if (selecionado >= MAX_M) {
        return 0;
    }
    
    while (1) {
        limpar_tela();
        // mostrar qual tela ta 
        Clic_moveToColumn(Clic_getScreenCenterColumn() - 18);
        Clic_printBox(36,3);
        Clic_moveUpBegin(1);
        Clic_moveRight(3);
        Clic_printCenter("ESCOLHA QUAL CARTA SUBSTITUIR\n");
        
        // carta puxada
        Clic_move(5,3);
        printf("Nova carta puxada:\n");
        Clic_moveToColumn(3);
        m_carta(baralho, &carta_nova);


        // mostrar sua mao
        Clic_moveDown(24);
        Clic_moveToColumn(3);
        printf("Sua mão atual:\n");
        Clic_moveToColumn(Clic_getScreenCenterColumn()-30);
        m_mao_selecionavel(baralho, sua_m, selecionado);
        
        // APENAS MOSTRA MÃO OCULTA SE TRAPACEIRO MODE ATIVO
        if (trapaceiro_mode && mostrar_oculta) {
            Clic_moveUp(22);
            Clic_moveToColumn(3);
            printf("Mão oculta:\n");

            Clic_moveToColumn(Clic_getScreenCenterColumn()-30);
            m_mao(baralho, 0, manga);
        }

        
        Clic_moveToColumn(3);


        // Mostra opções de manga apenas se trapaceiro mode ativo
        if (trapaceiro_mode) {
            Clic_moveToLine(altura - 8);

            Clic_printBox(44,6);
            Clic_moveUp(4);
            Clic_moveToColumn(4);
            Clic_moveToColumn(4);
            printf("↑ : Ver/Ocultar mão oculta\n");
            Clic_moveToColumn(4);
            printf(" ← → : Navegar  │  ENTER: Substituir\n");
            Clic_moveToColumn(4);
            printf("↓ : Adicionar à mão oculta\n");
            Clic_moveToColumn(4);
            printf("ESC : Descartar nova carta\n");
        }else{
            Clic_moveToLine(altura - 5);
            Clic_printBox(44,4);
            Clic_moveUp(2);
            Clic_moveToColumn(4);
            printf(" ← → : Navegar  │  ENTER: Substituir\n");
            Clic_moveToColumn(4);
            printf("ESC : Descartar nova carta\n");
        }

        key = Clic_keyCapture();

        // SETA PARA BAIXO - APENAS FUNCIONA SE TRAPACEIRO MODE ATIVO
        if (key == Key_DOWN && trapaceiro_mode) {
            adicionar_manga(baralho, carta_nova, manga, soma_oculta);
            return -2; // Código especial para indicar que foi para mão oculta

        // SETA PARA CIMA - APENAS FUNCIONA SE TRAPACEIRO MODE ATIVO
        } else if (key == Key_UP && trapaceiro_mode) {
            mostrar_oculta = !mostrar_oculta;

        } else if (key == Key_LEFT) {
            do {
                selecionado--;
                if (selecionado < 0) selecionado = 4;
            } while (sua_m[selecionado] == -1);
            
        } else if (key == Key_RIGHT) {
            do {
                selecionado++;
                if (selecionado > 4) selecionado = 0;
            } while (sua_m[selecionado] == -1);
            
        } else if (key == Key_ENTER) {
            return selecionado;
            
        } else if (key == 27) { // ESC - pede confirmação
            if (confirmar_descarte()) {
                return -1; // Confirma descarte
            }
            // Se não confirmar, continua no loop
        }
    }
}

void mostrar_ranking() {
    FILE *arquivo;
    EstatisticaJogador jogadores[100];
    int total_jogadores = 0;
    int i;
    arquivo = fopen("ranking.bin", "rb");
    
    if (arquivo == NULL) {
        Clic_moveToColumn(3);
        printf("\n  Nenhum jogador registrado ainda!\n\n");
        return;
    }
    
    // Lê todos os jogadores
    while (fread(&jogadores[total_jogadores], sizeof(EstatisticaJogador), 1, arquivo) == 1) {
        total_jogadores++;
        if (total_jogadores >= 100) break;
    }
    fclose(arquivo);
    
    if (total_jogadores == 0) {
        Clic_moveToColumn(3);
        printf("\n  Nenhum jogador registrado ainda!\n\n");
        return;
    }
    
    // Ordena por número de "NA MOSCA" (decrescente)
    for (int i = 0; i < total_jogadores - 1; i++) {
        for (int j = 0; j < total_jogadores - i - 1; j++) {
            if (jogadores[j].na_mosca_count < jogadores[j + 1].na_mosca_count) {
                EstatisticaJogador temp = jogadores[j];
                jogadores[j] = jogadores[j + 1];
                jogadores[j + 1] = temp;
            }
        }
    }
    
    Clic_printBox(largura - 3, 3);
    Clic_moveUpBegin(1);
    Clic_moveToColumn(Clic_getScreenCenterColumn()-28);
    printf("               RANKING DE JOGADORES                   \n");
    Clic_moveDownBegin(3);
    
    //  mostrar os legenda de infromações do jogo
    Clic_moveToColumn(Clic_getScreenCenterColumn()-37);
    Clic_printBox(72,3);

    Clic_moveUp(1);
    Clic_moveToColumn(Clic_getScreenCenterColumn()-31);
    printf("│ #  │  JOGADOR       │  VITORIA     │  TOTAL PARTIDAS│\n");


    Clic_moveDown(2);
    Clic_moveToColumn(Clic_getScreenCenterColumn()-37);
    Clic_printBox(72, total_jogadores + 2 );
    Clic_moveUp(total_jogadores);
   
    for (i = 0; i < total_jogadores; i++) {
        // Símbolo especial para top 3
        char simbolo[5] = "  ";
        if (i == 0) strcpy(simbolo, "🥇");
        else if (i == 1) strcpy(simbolo, "🥈");
        else if (i == 2) strcpy(simbolo, "🥉");
        

        Clic_moveToColumn(Clic_getScreenCenterColumn()-36);
        printf("  %s │ %-20s │      %3d    │      %3d       |\n",
               simbolo,
               jogadores[i].nickname,
               jogadores[i].na_mosca_count,
               jogadores[i].total_partidas
            );
    }

    
}
