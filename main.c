/*
gcc -o jogo main.c jogo.c funcao.c cartaView.c telas.c Clic.c -lm
*/
#include "funcao.h"

void main() {
    srand(time(NULL));
    int opcao_selecionada = 0;
    int key;

    while (verificar_tamanho_terminal()) {
        mostrar_menu(opcao_selecionada);
        
        // Captura tecla
        key = Clic_keyCapture();
        
        switch (key) {
            case Key_UP:
                opcao_selecionada--;
                if (opcao_selecionada < 0) opcao_selecionada = 2; // São 3 opções (0, 1, 2)
                break;
                
            case Key_DOWN:
                opcao_selecionada++;
                if (opcao_selecionada > 2) opcao_selecionada = 0; // São 3 opções (0, 1, 2)
                break;
                
            case Key_ENTER:
                executar_opcao(opcao_selecionada);
                break;
                
            default:
                break;
        }
    }
}