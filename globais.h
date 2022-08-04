#include <stdio.h>
#include <stdlib.h>

bool paused = false; //Trava as animações
int contador = 0; //Auxílio para quantidade de inimigos mortos
int life = 3; //Vida do jogador
bool continuar = false; //Auxilia na manipulação do pause
int contadorEnter = 0; //Contador de teclas para mudança de telas
int contadorEspera = 0; //Contador de delay
int contadorReset = 0; //Contador do reset para que a animação não seja chamada novamente, um retorno no tempo
int contadorCena = 0; //Auxilia no Boss
int vidaBoss = 15; //Vida do chefão
bool setas = false; //Travamento do teclado de setas
bool direcao_boss = true; // Alterna a direção x do chefão
bool mudaDirecao = false; // Alterna as descidas a partir da direção seguida
