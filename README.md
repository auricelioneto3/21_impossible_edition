# 21 Impossible Edition

## Descrição

Um jogo de cartas simples: puxe uma carta, descarte outra e siga assim até alcançar o limite — ou, pelo menos, chegar o mais perto possível dele.

## Conceitos

1. **Cartas**: a mecânica principal de jogabilidade, é por meio delas que se chega ao limite. Cada carta tem um valor de 1 a 10 e um naipe (copas, paus, ouros ou espadas), e o baralho é finito (40 cartas). Você soma as cartas da sua **mão** e, por meio dessa soma, obtém sua **pontuação** para tentar alcançar o limite.

2. **Limite (meta)**: um valor inicial que você precisa alcançar. Esse valor pode mudar ao longo da partida e, à primeira vista, pode até parecer impossível de bater.

3. **Penalidades**: modificadores do limite que surgem de forma aleatória e podem tornar o jogo mais difícil — ou até impossível de vencer. São elas:
   - sua meta recebe +10;
   - sua meta soma a maior carta da sua mão;
   - sua meta multiplica pela menor carta da sua mão.

4. **Tentativas**: depois que sua mão (5 cartas) estiver completa, você passa a ter 10 tentativas para trocar cartas até bater a meta.

5. **Ranking**: ao final de cada partida, seu resultado é salvo. O ranking mostra os jogadores que mais acertaram a meta "na mosca" (pontuação exatamente igual ao limite).

## Trapaça

Ao pressionar as teclas **cima, cima, baixo, baixo** logo após digitar seu nickname (antes de apertar ENTER para começar), você libera o **modo trapaceiro**, que ativa a **carta na manga**: um espaço oculto onde é possível guardar cartas puxadas. Cartas na manga não entram no cálculo das penalidades (que usa apenas a menor e a maior carta da mão visível), mas ainda são somadas à sua pontuação final.

## Requisitos

- Terminal compatível com sequências de escape ANSI, configurado para UTF-8.
- Tamanho mínimo recomendado: 100 colunas x 50 linhas.
- Interface construída com a biblioteca [CLIC](Manual_da_Biblioteca_CLIC.pdf).

## Como compilar

```bash
gcc -o jogo main.c jogo.c funcao.c cartaView.c telas.c Clic.c -lm
```

## Como executar

```bash
./jogo
```
