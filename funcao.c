#include "funcao.h"

int verificar_tamanho_terminal() {
    int largura_atual = Clic_getScreenWidth();
    int altura_atual = Clic_getScreenHeight();
    
    // Verifica se o terminal tem pelo menos 100 de largura e 50 de altura
    if (largura_atual < LARGURA_MINIMA || altura_atual < ALTURA_MINIMA) {
        system("clear");
        Clic_move(1, 1);
        printf("╔═══════════════════════════════════════════════════════════════╗\n");
        printf("║                    AVISO IMPORTANTE                           ║\n");
        printf("╚═══════════════════════════════════════════════════════════════╝\n\n");
        printf("  O tamanho atual do terminal é: %dx%d\n\n", largura_atual, altura_atual);
        printf("  Para uma experiência agradável, ajuste o terminal para\n");
        printf("  uma proporção de 100x50 ou mais.\n\n");
        printf("  Tamanho recomendado: 100 colunas x 50 linhas\n\n");
        printf("  Pressione ENTER para continuar mesmo assim...\n");
        printf("  (A interface pode não ser exibida corretamente)\n");
        getchar();

        return 0; // Terminal não está no tamanho ideal
        exit(0);
    }


    return 1;
    
     // Terminal está no tamanho adequado
}

/*=========== menu inicio =============== */

void executar_opcao(int opcao) {

    switch (opcao) {
        case 0: 

            jogo();
            break;
            
        case 1: // Ranking de jogadores
            limpar_tela();
            mostrar_ranking();
            Clic_moveToColumn(3);
            
            Clic_moveToLine(altura - 2);
            printf("Pressione ENTER para voltar...");
            getchar();
            break;
            
        case 2: // Sair
            limpar_tela();
            Clic_printCenter("  Obrigado por jogar! ate logo! ");
            Clic_moveDownBegin(altura);
            exit(0);
            break;
            
        default:
            break;
    }
}
/*========= game_funcionalidades ==========*/

void inicializar(struct carta baralho[MAX_B]) {
    int nape = 1, numero = 1, cont = 0;
    while (cont < MAX_B) {
        while (nape <= 4) {
            while (numero <= 10) {
                baralho[cont].naipe = nape;
                baralho[cont].numero = numero;
                baralho[cont].verificar = 0;
                numero++;
                cont++;
            }
            nape++;
            numero = 1;
        }
    }
}

int puxar_cartar(struct carta baralho[MAX_B]) {
    int i = rand() % MAX_B;
    while (baralho[i].verificar) {
        i = rand() % MAX_B;
    }
    baralho[i].verificar = 1;
    return i;
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int confirmar_descarte() {
    int selecionado = 0; // 0 = Não, 1 = Sim
    int key;
    
    while (1) {
        limpar_tela();
        
        Clic_moveToColumn(3);
        printf("╔═══════════════════════════════════════╗\n");
        Clic_moveToColumn(3);
        printf("║   DESCARTAR A CARTA PUXADA?           ║\n");
        Clic_moveToColumn(3);
        printf("╚═══════════════════════════════════════╝\n\n");
        
        Clic_moveToColumn(3);
        printf("Tem certeza que deseja descartar esta carta?\n");
        Clic_moveToColumn(3);
        printf("Ela será perdida permanentemente!\n\n");
        
        // Opções
        if (selecionado == 0) {
            Clic_moveToColumn(3);
            printf("  > [NÃO]  Sim\n");
        } else {
            Clic_moveToColumn(3);
            printf("    Não  > [SIM]\n");
        }
        
        Clic_moveToColumn(3);
        printf("\n┌──────────────────────────────────────────┐\n");
        Clic_moveToColumn(3);
        printf("│ ← → : Navegar  │  ENTER: Confirmar      │\n");
        Clic_moveToColumn(3);
        printf("│ ESC : Cancelar                          │\n");
        Clic_moveToColumn(3);
        printf("└──────────────────────────────────────────┘\n");
        
        key = Clic_keyCapture();
        
        if (key == Key_LEFT || key == Key_RIGHT) {
            selecionado = !selecionado;
        } else if (key == Key_ENTER) {
            return selecionado; // 0 = Não descartar, 1 = Descartar
        } else if (key == 27) { // ESC cancela e volta
            return 0; // Não descartar
        }
    }
}

int mao_cheia(int sua_m[MAX_M]) {
    for (int j = 0; j < MAX_M; j++) {
        if (sua_m[j] == -1) {
            return 0; // Tem espaço vazio
        }
    }
    return 1; // Mão está cheia
}

int calcular_soma_total(struct carta baralho[MAX_B], int sua_m[MAX_M], int manga[MAX_M], int trapaceiro_mode) {
    int soma = 0;
    
    // Soma das cartas visíveis
    for (int j = 0; j < MAX_M; j++) {
        if (sua_m[j] != -1) {
            soma += baralho[sua_m[j]].numero;
        }
    }
    
    // Soma das cartas ocultas - APENAS SE TRAPACEIRO MODE ATIVADO
    if (trapaceiro_mode) {
        for (int j = 0; j < MAX_M; j++) {
            if (manga[j] != -1) {
                soma += baralho[manga[j]].numero;
            }
        }
    }
    
    return soma;
}

void min_max_mao(struct carta baralho[MAX_B], int sua_m[MAX_M], int *menor, int *maior) {
    *menor = -1;
    *maior = -1;
    
    for (int j = 0; j < MAX_M; j++) {
        if (sua_m[j] != -1) {
            int valor = baralho[sua_m[j]].numero;
            
            if (*menor == -1 || valor < *menor) {
                *menor = valor;
            }
            
            if (*maior == -1 || valor > *maior) {
                *maior = valor;
            }
        }
    }
}

void salva_c(int *ind, int sua_m[MAX_M], int i, int *somar_visivel, struct carta baralho[MAX_B], int manga[MAX_M], int *soma_oculta, int trapaceiro_mode) {
    
    int posicao;
    
    // Usa a função mao_cheia para verificar se há espaço
    if (!mao_cheia(sua_m)) {
        // Mão não está cheia - pergunta se quer salvar
        Clic_fixDraw();
        Clic_moveUpBegin(3);
        Clic_moveToColumn(3);
        printf("Deseja salvar essa carta? (1-Sim / 0-Não): ");
        int escolha;
        scanf("%d", &escolha);
        getchar(); // Limpa buffer
        
        if (escolha) {
            // Adiciona na primeira posição vazia
            for (int j = 0; j < MAX_M; j++) {
                if (sua_m[j] == -1) {
                    sua_m[j] = i;
                    break;
                }
            }
        }
    } else {
        // Mão cheia - escolhe qual substituir
        posicao = escolher_posicao_mao(baralho, sua_m, i, manga, soma_oculta, trapaceiro_mode);
        
        if (trapaceiro_mode) {
            if (posicao == -2) {
                // Carta foi para mão oculta, não faz nada aqui
                return;
            }
        }
        
        if (posicao != -1) {
            sua_m[posicao] = i;
        }
    }
    
    // Recalcula soma
    *somar_visivel = 0;
    for (int j = 0; j < MAX_M; j++) {
        if (sua_m[j] != -1)
            *somar_visivel += baralho[sua_m[j]].numero;
    }
}

/*========== SISTEMA DE RANKING ==========*/

void atualizar_estatisticas(char *nickname, int acertou_na_mosca) {
    FILE *arquivo;
    EstatisticaJogador jogadores[100];
    int total_jogadores = 0;
    int jogador_encontrado = 0;
    
    // Lê as estatísticas existentes
    arquivo = fopen("ranking.bin", "rb");
    if (arquivo != NULL) {
        while (fread(&jogadores[total_jogadores], sizeof(EstatisticaJogador), 1, arquivo) == 1) {
            total_jogadores++;
            if (total_jogadores >= 100) break;
        }
        fclose(arquivo);
    }
    
    // Procura o jogador
    for (int i = 0; i < total_jogadores; i++) {
        if (strcmp(jogadores[i].nickname, nickname) == 0) {
            jogadores[i].total_partidas++;
            if (acertou_na_mosca) {
                jogadores[i].na_mosca_count++;
            }
            jogador_encontrado = 1;
            break;
        }
    }
    
    // Se não encontrou, adiciona novo jogador
    if (!jogador_encontrado) {
        strcpy(jogadores[total_jogadores].nickname, nickname);
        jogadores[total_jogadores].total_partidas = 1;
        jogadores[total_jogadores].na_mosca_count = acertou_na_mosca ? 1 : 0;
        total_jogadores++;
    }
    
    // Salva as estatísticas atualizadas
    arquivo = fopen("ranking.bin", "wb");
    if (arquivo != NULL) {
        fwrite(jogadores, sizeof(EstatisticaJogador), total_jogadores, arquivo);
        fclose(arquivo);
    }
}
/*=============== mecanica ===============*/

void penalidade(int *meta, int c_menor, int c_alta, int tentativa) {
    if (*meta >= 49 || tentativa <= 0) {
        return;
    }
    
    limpar_tela();
    Clic_moveToColumn(3);
    printf("Ops, acho que me confundi...\n");
    getchar();
    limpar_tela();
    
    int trapaca = rand() % 10;
    if (trapaca < 4) {
        switch (trapaca) {
            case 1:
            Clic_moveToColumn(3);    
            printf("sua meta recebe +10\n");
                *meta += 10;
                break;
            case 2:
            Clic_moveToColumn(3);    
            printf("sua meta + maior carta\n");
                *meta += c_alta;
                break;
            case 3:
            Clic_moveToColumn(3);    
            printf("sua meta * menor carta\n");
                *meta *= c_menor;
                break;
        }
    } else {
        Clic_moveToColumn(3);
        printf("Ah não, tá tudo certo\n");
    }
    getchar();
}

/*============aqui COMEÇA A SAFADEZA ===================*/

void adicionar_manga(struct carta baralho[MAX_B], int carta_puxada, int manga[MAX_M], int *soma_oculta) {
    // Procura o primeiro espaço vazio na mão oculta
    for (int j = 0; j < MAX_M; j++) {
        if (manga[j] == -1) {
            manga[j] = carta_puxada;  // Adiciona a carta que FOI PUXADA
            *soma_oculta += baralho[carta_puxada].numero;  // Atualiza a soma
            Clic_moveToColumn(3);
            printf("\n[Carta adicionada à mão oculta!]\n");
            getchar();
            break;  // Adiciona apenas UMA carta e para
        }
    }
}



int verificar_trapaca(){
    int sequencia_alvo[] = {Key_UP, Key_UP, Key_DOWN, Key_DOWN, Key_LEFT,Key_RIGHT,Key_LEFT,Key_RIGHT}; 
    int progresso_seq = 0;
    int tecla = 0;
    
    // Loop captura teclas até que ENTER seja pressionado
    while (true) {
        tecla = Clic_keyCapture();

        if (tecla == Key_ENTER) {
            return 0; // Pressionou ENTER sem completar a sequência
        }

        // Verifica a sequência para o modo trapaceiro
        if (tecla == sequencia_alvo[progresso_seq]) {
            progresso_seq++;
            
            // Se completou a sequência (8 teclas corretas)
            if (progresso_seq == 8) {
                Clic_breakLine();
                Clic_moveToColumn(3);
                Clic_setFontColor(Color_RED);
                printf(">> MODO TRAPACEIRO ATIVADO! <<");
                Clic_resetColor();
                Clic_breakLine();
                Clic_moveToColumn(3);
                printf("Pressione ENTER para continuar...");
                getchar(); // Espera confirmação
                return 1;
            }
        } else {
            progresso_seq = 0; // Errou a sequência, reseta
            // Se errou mas apertou CIMA, pode ser o início de uma nova tentativa
            if (tecla == Key_UP) {
                progresso_seq = 1;
            }
        }
    }
}