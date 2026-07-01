/* GLOSSARIO DE FUNCOES

    MENU INICIO
        - mostrar_menu:
        - exercultar_opcoes

    FUNCIONALIDADES
        - inicializar: 
            inicializa o baralho, 40 cartas dividida em 4 nape que vao de 1 a 10;

        - limpar_tela: 
            limpa a tela;

        - puxar_carta:
            ele seleciona uma carta aleatoria, que seja valida[baralho.verificar = 0]
            dentro do baralho e marca ela [baralho.verificar = 1], para entrar no loop;
        
        - salvar_c:
            - verficiar se tem espaco livre
            - se tiver espaco, a cartar e postar na mao
            - se NAO tiver espaco, abri telar de selacao
            - caso o jogador coloque a carta na mao oculta ela e salva nela
        
        - min_max_c:
            pesquisa a menor e a menor carta
    
       - adicionar_manga:
        se a mao oculta tiver espaco, adiconar 
    

        - calcular somar tortal:
        ele rertona a soma da mao_principal com a mao ocuta
    - 
            
    MECANICAS 
        - escolher_posicao_mao: 
            mostrar a tela de selecao de cartas, ler a teclar apertada
            * ocultar a manga
            * Separar uma funcao que mostrer a tela de selecao
            
            
        - sitema_trapaca:
            - tela de trapaca
            - sorteia se o jogador vai ou tomar alguma penalidade
    
    VIEW

        - m_carta:
            mostra carta em sentido VERTICAL
        
        - m_mao:
            mostra carta em senti HORIZONTAL

        -m_mao selecionada:
            destaca a mao seleecionada
        - mensagem final: 
            mostrar se vc perdeu, ganhou, chegou perto
        - placa:
            mostrar o placa
        
*/

#ifndef FUNCAO_H
#define FUNCAO_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Clic.h"

#define LARGURA_MINIMA 100
#define ALTURA_MINIMA 48
#define MAX_B 40 //maximo de cartas em um baralho
#define MAX_M 5  // maximo de cartas em uma mao
#define largura  Clic_getScreenWidth()
#define altura  Clic_getScreenHeight()-2

    

// ===========  CARTA    =====================
struct carta {
    int verificar; // 1 = nao foi jogada; 0 = ja foi;
    int numero;    // 1 a 10
    int naipe;     // 1 = copas, 2 = paus, 3 = ouros, 4 = espadas;
};

// Estatísticas dos jogadores - APENAS RANKING
typedef struct {
    char nickname[30];
    int na_mosca_count;  // Quantas vezes acertou exatamente
    int total_partidas;  // Total de partidas jogadas
} EstatisticaJogador;

int jogo();

// Declaracoes das funcoes

/*=============menu de inicio ===========*/
int verificar_tamanho_terminal();
void logo();
void mostrar_menu(int opcao_selecionada);
void executar_opcao(int opcao);


/*============funcionalidade=============*/
void inicializar(struct carta baralho[MAX_B]);
void limpar_tela();
int puxar_cartar(struct carta baralho[MAX_B]);
int verificar_trapaca();
int mao_cheia(int sua_m[MAX_M]);
void salva_c(int *ind, int sua_m[MAX_M], int i, int *somar_visivel, struct carta baralho[MAX_B], int manga[MAX_M], int *soma_oculta, int trapaceiro_mode);
void min_max_mao(struct carta baralho[MAX_B], int sua_m[MAX_M], int *menor, int *maior);

// SISTEMA DE RANKING
void atualizar_estatisticas(char *nickname, int acertou_na_mosca);
void mostrar_ranking();

/*============== mecanicas ===================*/
void mensagem_f(int somar_visivel, int meta);
void adicionar_manga(struct carta baralho[MAX_B], int carta_puxada, int manga[MAX_M], int *soma_oculta);
int calcular_soma_total(struct carta baralho[MAX_B], int sua_m[MAX_M], int manga[MAX_M], int trapaceiro_mode);
int confirmar_descarte();
void penalidade(int *meta, int c_menor, int c_alta, int tentativa);

/*================== view =====================*/
void m_carta(struct carta baralho[MAX_B], int *i);
void m_mao_selecionavel(struct carta baralho[MAX_B], int sua_m[MAX_M], int selecionado);
void m_mao(struct carta baralho[MAX_B], int i, int sua_m[MAX_M]);
void placa (int tentativa, int meta, int soma_total);
void criar_borda_jogo();

/* ============ telas do jogo =================*/
void tela_principal(int tentativa, int meta, int soma_total, struct carta baralho[MAX_B], int i, int sua_m[MAX_M], int *resposta);
void tela_mostra_carta(int tentativa, int meta, int soma_total, struct carta baralho[MAX_B], int i, int sua_m[MAX_M], int *somar_visivel, int manga[MAX_M], int *soma_oculta, int trapaceiro_mode);
int escolher_posicao_mao(struct carta baralho[MAX_B], int sua_m[MAX_M], int carta_nova, int manga[MAX_M], int *soma_oculta, int trapaceiro_mode);


#endif // FUNCAO_H