#include "funcao.h"

int jogo() {
    struct carta baralho[MAX_B];
    
    int c_alta, c_menor;
    char nickname[10];
    int sua_m[MAX_M] = {-1, -1, -1, -1, -1};
    int manga[MAX_M] = {-1, -1, -1, -1, -1};
    int tentativa = 10, resposta;
    int somar_visivel = 0, soma_oculta = 0, i;
    int meta = 20 + rand() % 30;
    int verificar = 1;
    int trapaceiro_mode = 0;

    // inicializa o baralho
    inicializar(baralho);
    limpar_tela();
    
    //tela de "login"
    Clic_moveToColumn(3);// para nao inteferir na borda 
    printf("BEM-VINDO AO JOGO DE CARTAS!\n");
    Clic_moveToColumn(3);
    printf("antes de mais nada, qual o seu nickname: ");
    scanf("%s", nickname);

    // aviso sobre o funcionamento do jogo
    limpar_tela();
    Clic_moveToColumn(3);
    printf("puxe cartas, decida ser vai ficar ou não e alcance a meta\n");
    Clic_moveToColumn(3);
    printf("As primeiras 5 cartas da sua mão são por conta da casa, depois disso você perde tentativa\n");
    Clic_breakLine();
    Clic_moveToColumn(3);
    printf("Pressione ENTER para começa...");
    getchar();

    //ligar a bagaçeira
    trapaceiro_mode = verificar_trapaca();

    // o jogador tem 10 tentativas apos ter escolhido suas primeiras 5 cartas na sua mao
    while (tentativa > 0) {
        int soma_total = calcular_soma_total(baralho, sua_m, manga, trapaceiro_mode); // faz a soma total, caso esteja ativado, o manga tbm serão considerados
        
        // tela principal onde tem as informações 
        limpar_tela();
        tela_principal(tentativa, meta, soma_total, baralho, i, sua_m, &resposta);
      
        // Se escolher 0, terminar o jogo
        if (resposta == 0) {
            break; // Sai do loop
        }

        // define qual carta vai ser ofertada 
        i = puxar_cartar(baralho);

        // mostrar a carta e define se vai salva ou não
        tela_mostra_carta(tentativa, meta, soma_total, baralho, i, sua_m, &somar_visivel, manga, &soma_oculta, trapaceiro_mode);
        
        // faz uma varredura apenas na sua mao visivel e procura a menor e maior carta
        min_max_mao(baralho, sua_m, &c_menor, &c_alta);  
        getchar();
        
        // penalidade
        penalidade(&meta, c_menor, c_alta, tentativa);
        
        // apenas para verififcar se a mao do jogador estar cheia para liberar a contagem
        if (mao_cheia(sua_m)) {
            if (verificar) {
                limpar_tela();
                Clic_moveToColumn(3);
                printf("  A partir de agora é por sua conta!\n");
                Clic_moveToColumn(3);
                printf("  você tem 10 chances de errar.\n");
                Clic_moveToColumn(3);
                printf("  Boa sorte!\n");
                Clic_moveToColumn(3);
                printf("Pressione ENTER para continuar...");
                getchar();
                verificar = 0;
            }
            tentativa--;
        }
    }
    
    limpar_tela();
    int soma_total_final = calcular_soma_total(baralho, sua_m, manga, trapaceiro_mode);
    mensagem_f(soma_total_final, meta);
    
    // Mostra resultado da mÃ£o oculta se trapaceiro mode ativo
    if (trapaceiro_mode && soma_oculta > 0) {
        Clic_moveToColumn(3);
        printf("Cartas na manga: %d pontos escondidos!\n", soma_oculta);
    }
    
    // Verifica se acertou "NA MOSCA"
    int acertou_na_mosca = (soma_total_final == meta) ? 1 : 0;
    
    // Atualiza o ranking
    atualizar_estatisticas(nickname, acertou_na_mosca);
    
    if (acertou_na_mosca) {
        Clic_moveToColumn(3);
        printf(" Seu acerto foi registrado no ranking!\n");
    } else {
        Clic_moveToColumn(3);
        printf(" Partida registrada no ranking!\n");
    }
    
    getchar();
    
    return 0;
}