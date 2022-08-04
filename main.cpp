#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h>
#include <string>
#include <time.h>
#include <SOIL/SOIL.h>

/*
Os Headers são utilizados para separar o código, no entanto eles tem um padrão, ambos são formados
pela mesma estratégia, a soma entre comprimento e a posição do corpo além da soma entre a altura e sua posição
em y, isso nos auxilia na construção da colisão do corpo, pois é possível retorna, como Classes, visto em Java,
esses itens que sofrem diversas alterações
*/

#include "player.h"
#include "vida.h"
#include "enemy.h"
#include "projetil_inimigo.h"
#include "pause.h"
#include "projetil_player.h"
#include "projetil_boss.h"
#include "globais.h"
#include "texturas.h"
#include "boss.h"
#undef main

/*
Todas as telas tem seus desenhos representados por texturas, dessa forma, a manipulação
das imagens é realizada no displayFunc que irá sempre se repetir, consequentemente, desenhaMinhaCena
tem uma grande quantidade de linhas porque esta comporta cinco cenas diferentes, que são sobrescritas
*/

void inicializa(); //Todos os itens da cena são inicializados mas não criados
void desenhaMinhaCena(); //Criação de itens da tela principal do jogo (Contém o jogo, a tela de preparo, derrota e vitória)
void desenhaTelaInicial(); //Criação da splash screen
void desenhaTelaInstrucoes(); //Criação da tela de instruções
void deteccaoTeclado(int chave, int x, int y); //Movimentação do player a partir do GLUT
void ascii(unsigned char key, int x, int y); //Teclas espaço, R, P, Enter
void moveInimigo(int local); //Movimentação do inimigo (x,y)
void moveBalaInimiga1(int b); //Movimentação e reinício de projétil inimigo (x,y)
void moveBalaInimiga2(int a); //Movimentação e reinício de projétil inimigo (x,y)
void moveBoss(int c); //Movimentação do chefão (x)
void moveBalaBoss(int a); //Movimentação e reinício do projétil do chefão (x,y)
//Carregador - Comporta dois projéteis
void moveBalaPlayer(int c); //Movimentação e reinício do projétil do player (x,y)
void moveBalaPlayer1(int d); //Movimentação e reinício do projétil do player (x,y)
void atualiza(); //Redesenha a tela atual
void delay(int milliseconds); //Trava a tela - ideal para telas de preparo
//Colisão inimigo com player
//boleano Função(posição x do inimigo, posição y do inimigo, comprimento do inimigo, altura do inimigo, posição x do jogador, posição y do jogador, altura do jogador, comprimento do jogador)
bool colisao(float inimX,float inimY,float inimCOMP, float inimALT, float playerX, float playerY, float playerALT, float playerCOMP);
//booleano Função(posição da bala do player no eixo x, posição da bala do player no eixo y, comprimento do projétil, altura do projétil, posição x do inimigo, posição y do inimigo, comprimento do inimigo, altura do inimigo)
bool colisaoBalaPlayer(float playerBX,float playerBY,float playerBCOMP, float playerBALT, float inimX, float inimY, float inimALT, float inimCOMP);
bool colisaoBalaInimigo(float inimBX,float inimBY,float inimBCOMP, float inimBALT, float playerX, float playerY, float playerALT, float playerCOMP);

int main(int argc, char** argv) {

    //SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO); //Inicia o áudio do SDL em todo projeto
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ){
       printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    }
    //Abertura de áudio a partir do SDL, dessa forma, toda vez que o áudio for fechado, Mix_OpenAudio deve ser iniciado
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
       printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }

    //GLUT
    glutInit(&argc, argv); //Inicializa o glut
    glutInitContextVersion(1, 1); //Compatibilidade com o Hardware
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE); //Especifica o uso de cores e buffers
    glutInitWindowPosition(30, 0); //Especifica aonde a janela aparece na tela
    glutInitWindowSize(1280, 720); //Especifica as dimensoes da janela
    glutCreateWindow("Doom - Galaxian");
    glutFullScreen();

    inicializa(); //Coloca na memória todas os fatores que pretendem ser criados

    //GLUT especial
    glutSpecialFunc(deteccaoTeclado); // Esquerda e direita pelo Glut
    glutKeyboardFunc(ascii); //Enter, P,R, Espaço
    glutDisplayFunc(desenhaTelaInicial); //Carregamento da tela início

    //Finalizações da Main
    Mix_CloseAudio(); //Fecha o áudio
    glutMainLoop();
    SDL_Quit(); //Interrompe a instância iniciada pelo SDL_Init
    return 0;
}

void atualiza() {
    glutPostRedisplay();
}

void desenhaTelaInicial(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Limpa a janela e profundidade
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100, 100, -100, 100, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glColor3f (1, 1, 1);

    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaFundo);
    glBegin(GL_TRIANGLE_FAN);
        // Associamos um canto da textura para cada vértice
        glTexCoord2f(0, 0);
        glVertex3f(-100, -100,  0);
        glTexCoord2f(1, 0);
        glVertex3f( 100, -100,  0);
        glTexCoord2f(1, 1);
        glVertex3f( 100,  100,  0);
        glTexCoord2f(0, 1);
        glVertex3f(-100,  100,  0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(0,-75,0);
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaOp1);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(-50, -7, 0);
        glTexCoord2f(1, 0);
        glVertex3f(50, -7, 0);
        glTexCoord2f(1, 1);
        glVertex3f(50, 7, 0);
        glTexCoord2f(0, 1);
        glVertex3f(-50, 7, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(0,50,0);
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaGalaxian);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(-14, -7, 0);
        glTexCoord2f(1, 0);
        glVertex3f(14, -7, 0);
        glTexCoord2f(1, 1);
        glVertex3f(14, 7, 0);
        glTexCoord2f(0, 1);
        glVertex3f(-14, 7, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

void desenhaTelaInstrucoes(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Limpa a janela e profundidade
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100, 100, -100, 100, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glColor3f (1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaInstrucoes);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0);
        glVertex3f(-100, -100,  0);
        glTexCoord2f(1, 0);
        glVertex3f( 100, -100,  0);
        glTexCoord2f(1, 1);
        glVertex3f( 100,  100,  0);
        glTexCoord2f(0, 1);
        glVertex3f(-100,  100,  0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(-25,77,0);
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaPalavrasInstrucoes);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(-30, -7, 0);
        glTexCoord2f(1, 0);
        glVertex3f(30, -7, 0);
        glTexCoord2f(1, 1);
        glVertex3f(30, 7, 0);
        glTexCoord2f(0, 1);
        glVertex3f(-30, 7, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(0,50,0);
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, idTexturaInstrucao1);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(-57, -7, 0);
        glTexCoord2f(1, 0);
        glVertex3f(57, -7, 0);
        glTexCoord2f(1, 1);
        glVertex3f(57, 7, 0);
        glTexCoord2f(0, 1);
        glVertex3f(-57, 7, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(-10,22,0);
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, idTexturaInstrucao2);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(-46.5, -8, 0);
        glTexCoord2f(1, 0);
        glVertex3f(46.5, -8, 0);
        glTexCoord2f(1, 1);
        glVertex3f(46.5, 8, 0);
        glTexCoord2f(0, 1);
        glVertex3f(-46.5, 8, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(-19.5,0,0);
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, idTexturaInstrucao3);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(-37, -5, 0);
        glTexCoord2f(1, 0);
        glVertex3f(37, -5, 0);
        glTexCoord2f(1, 1);
        glVertex3f(37, 5, 0);
        glTexCoord2f(0, 1);
        glVertex3f(-37, 5, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(-16.5,-25,0);
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, idTexturaInstrucao4);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(-40, -5, 0);
        glTexCoord2f(1, 0);
        glVertex3f(40, -5, 0);
        glTexCoord2f(1, 1);
        glVertex3f(40, 5, 0);
        glTexCoord2f(0, 1);
        glVertex3f(-40, 5, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(0,-50,0);
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, idTexturaInstrucao5);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(-56.3, -5.5, 0);
        glTexCoord2f(1, 0);
        glVertex3f(56.3, -5.5, 0);
        glTexCoord2f(1, 1);
        glVertex3f(56.3, 5.5, 0);
        glTexCoord2f(0, 1);
        glVertex3f(-56.3, 5.5, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(0,-75,0);
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, idTexturaPress);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(-20, -5, 0);
        glTexCoord2f(1, 0);
        glVertex3f(20, -5, 0);
        glTexCoord2f(1, 1);
        glVertex3f(20, 5, 0);
        glTexCoord2f(0, 1);
        glVertex3f(-20, 5, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

// É replicado no glutDisplayFunc, ou seja, fica em loop
void desenhaMinhaCena() {
    if(!paused){ //Anula o loop no pause
        if(contadorEspera == 0){ //Alterna janelas
            glPushMatrix();
            glColor3f (1, 1, 1);
            glEnable(GL_TEXTURE_2D);
            Mix_Music *narrador = Mix_LoadMUS("narrador.wav");
            Mix_PlayMusic(narrador, 0);
            Mix_Music *background = Mix_LoadMUS("funciona.wav");

            glBindTexture(GL_TEXTURE_2D, idTexturaEspera);
            glBegin(GL_TRIANGLE_FAN);
                glTexCoord2f(0, 0);
                glVertex3f(-100, -100,  0);
                glTexCoord2f(1, 0);
                glVertex3f( 100, -100,  0);
                glTexCoord2f(1, 1);
                glVertex3f( 100,  100,  0);
                glTexCoord2f(0, 1);
                glVertex3f(-100,  100,  0);
            glEnd();
            glPopMatrix();
            glDisable(GL_TEXTURE_2D);
            contadorEspera++;
            contadorReset++;
            glutPostRedisplay();
            glutSwapBuffers();
            delay(5000);
            Mix_PlayMusic(background, -1);
        }
        else if(contadorEspera == 1){ //Tela após a preparação
            continuar = true;
            setas = true; //Habilitação das setas
            contadorReset = 0; //Manipulação da tecla R
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-100, 100, -100, 100, -1, 1);
            glMatrixMode(GL_MODELVIEW);

            glPushMatrix();
            glColor3f(1, 1, 1);
            glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, idTexturaMoldura);
            glBegin(GL_TRIANGLE_FAN);
                glTexCoord2f(0, 0);
                glVertex3f(-100, -100, 0);
                glTexCoord2f(1, 0);
                glVertex3f(100, -100, 0);
                glTexCoord2f(1, 1);
                glVertex3f(100, 82, 0);
                glTexCoord2f(0, 1);
                glVertex3f(-100, 82, 0);
            glEnd();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(0,92,0);
            glColor3f(1, 1, 1);
            glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, idTexturaCab);
            glBegin(GL_POLYGON);
                glTexCoord2f(0, 0);
                glVertex3f(-30, -5, 0);
                glTexCoord2f(1, 0);
                glVertex3f(30, -5, 0);
                glTexCoord2f(1, 1);
                glVertex3f(30, 5, 0);
                glTexCoord2f(0, 1);
                glVertex3f(-30, 5, 0);
            glEnd();
            glPopMatrix();
            glDisable(GL_TEXTURE_2D);

            glPushMatrix();
            glColor3f (1, 1, 1);

            glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, idTexturaDoom);
            glBegin(GL_TRIANGLE_FAN);
                glTexCoord2f(0, 0);
                glVertex3f(-88, -80,  0);
                glTexCoord2f(1, 0);
                glVertex3f( 88, -80,  0);
                glTexCoord2f(1, 1);
                glVertex3f( 88,  80,  0);
                glTexCoord2f(0, 1);
                glVertex3f(-88,  80,  0);
            glEnd();
            glPopMatrix();
            glDisable(GL_TEXTURE_2D);

            glLoadIdentity();
            criar_player();
            for(int i=0; i<27; i++){
                criar_inimigo(i);
            }
            int vetor[5] = {13,23,25,1,15}; // Predefinição dos inimigos que serão ativos
            for(int i=0; i<5; i++){
                criar_projeteis_inimigos(vetor[i]);
            }
            int vetor4[5] = {3,4,8,9,20};
            for(int i=0; i<5; i++){
                criar_projeteis_inimigos(vetor4[i]);
            }
            // A variedade de tiros se dará pelo tempo de tela
            criar_projeteis_player(0);
            criar_projeteis_player(1);
            criar_vida(0);
            criar_vida(1);
            criar_vida(2);
            // Qualquer criar_X, ficará em loop, dessa forma é necessário apenas iniciá-lo

            //Derrota
            for (int i = 0; i<27; i++){
                if(life == -1 || pegar_ybaixo(i) == -80 || colisao(pegar_x_inimigo(i),pegar_ybaixo(i),inimigo[i].comprimento, inimigo[i].altura,pegar_xdir(),player.y,player.altura, player.comprimento) == true){
                    contador = 0;
                    contadorReset = 0;
                    contadorEspera++;
                    continuar = true;
                    paused = false;
                    contadorEnter = 3;
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Limpa a janela e profundidade
                    glMatrixMode(GL_PROJECTION);
                    glLoadIdentity();
                    glOrtho(-100, 100, -100, 100, -1, 1);
                    glMatrixMode(GL_MODELVIEW);

                    glPushMatrix();
                    glColor3f (0, 0, 0);
                    glBegin(GL_TRIANGLE_FAN);
                        glVertex3f(-100, -100,  0);
                        glVertex3f( 100, -100,  0);
                        glVertex3f( 100,  100,  0);
                        glVertex3f(-100,  100,  0);
                    glEnd();
                    glPopMatrix();

                    glPushMatrix();
                    glTranslatef(0,40,0);
                    glColor3f(1, 1, 1);

                    glEnable(GL_TEXTURE_2D);

                    glBindTexture(GL_TEXTURE_2D, idTexturaDerrota1);
                    glBegin(GL_POLYGON);
                        glTexCoord2f(0, 0);
                        glVertex3f(-50, -6, 0);
                        glTexCoord2f(1, 0);
                        glVertex3f(50, -6, 0);
                        glTexCoord2f(1, 1);
                        glVertex3f(50, 6, 0);
                        glTexCoord2f(0, 1);
                        glVertex3f(-50, 6, 0);
                    glEnd();
                    glPopMatrix();
                    glDisable(GL_TEXTURE_2D);

                    glPushMatrix();
                    glTranslatef(0,8,0);
                    glColor3f(1, 1, 1);

                    glEnable(GL_TEXTURE_2D);

                    glBindTexture(GL_TEXTURE_2D, idTexturaDerrota2);
                    glBegin(GL_POLYGON);
                        glTexCoord2f(0, 0);
                        glVertex3f(-25, -5, 0);
                        glTexCoord2f(1, 0);
                        glVertex3f(25, -5, 0);
                        glTexCoord2f(1, 1);
                        glVertex3f(25, 5, 0);
                        glTexCoord2f(0, 1);
                        glVertex3f(-25, 5, 0);
                    glEnd();
                    glPopMatrix();
                    glDisable(GL_TEXTURE_2D);

                    glPushMatrix();
                    glTranslatef(0,-35,0);
                    glColor3f(1, 1, 1);

                    glEnable(GL_TEXTURE_2D);

                    glBindTexture(GL_TEXTURE_2D, idTexturaDerrota3);
                    glBegin(GL_POLYGON);
                        glTexCoord2f(0, 0);
                        glVertex3f(-22, -7, 0);
                        glTexCoord2f(1, 0);
                        glVertex3f(22, -7, 0);
                        glTexCoord2f(1, 1);
                        glVertex3f(22, 7, 0);
                        glTexCoord2f(0, 1);
                        glVertex3f(-22, 7, 0);
                    glEnd();
                    glPopMatrix();
                    glDisable(GL_TEXTURE_2D);

                    glPushMatrix();
                    glTranslatef(80,-95,0);
                    glColor3f(1, 1, 1);
                    glEnable(GL_TEXTURE_2D);

                    glBindTexture(GL_TEXTURE_2D, idTexturaPressEnter);
                    glBegin(GL_POLYGON);
                        glTexCoord2f(0, 0);
                        glVertex3f(-15, -3, 0);
                        glTexCoord2f(1, 0);
                        glVertex3f(15, -3, 0);
                        glTexCoord2f(1, 1);
                        glVertex3f(15, 3, 0);
                        glTexCoord2f(0, 1);
                        glVertex3f(-15, 3, 0);
                    glEnd();
                    glPopMatrix();
                    glDisable(GL_TEXTURE_2D);

                    glPushMatrix();
                    glTranslatef(-80,-95,0);
                    glColor3f(1, 1, 1);
                    glEnable(GL_TEXTURE_2D);

                    glBindTexture(GL_TEXTURE_2D, idTexturaPressR);
                    glBegin(GL_POLYGON);
                        glTexCoord2f(0, 0);
                        glVertex3f(-15, -3, 0);
                        glTexCoord2f(1, 0);
                        glVertex3f(15, -3, 0);
                        glTexCoord2f(1, 1);
                        glVertex3f(15, 3, 0);
                        glTexCoord2f(0, 1);
                        glVertex3f(-15, 3, 0);
                    glEnd();
                    glPopMatrix();
                    glDisable(GL_TEXTURE_2D);
                }
            }
            //Encerramento
            if(contador == 27){
                setas = false;
                Mix_CloseAudio(); //Interrupção da música anterior
                Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
                Mix_Music *warning = Mix_LoadMUS("warning.wav");
                Mix_PlayMusic(warning, 0);

                glPushMatrix();
                glTranslatef(0,7.5,0);
                glColor3f(1, 1, 1);
                // Habilita o uso de texturas
                glEnable(GL_TEXTURE_2D);

                // Começa a usar a textura que criamos
                glBindTexture(GL_TEXTURE_2D, idTexturaWarning);
                glBegin(GL_POLYGON);
                    glTexCoord2f(0, 0);
                    glVertex3f(-50, -15, 0);
                    glTexCoord2f(1, 0);
                    glVertex3f(50, -15, 0);
                    glTexCoord2f(1, 1);
                    glVertex3f(50, 15, 0);
                    glTexCoord2f(0, 1);
                    glVertex3f(-50, 15, 0);
                glEnd();
                glPopMatrix();
                glDisable(GL_TEXTURE_2D);
                glutSwapBuffers();
                delay(4000);
                setas = true;
                Mix_Music *tema_boss = Mix_LoadMUS("tema_boss.wav");
                Mix_PlayMusic(tema_boss, -1);
                iniciar_boss(15,15);
                criar_boss();
                contador++;
                iniciar_projeteis_Boss(0,chefe.x,chefe.y);
                criar_projeteis_Boss(0);
                if(contadorCena == 0){ //Anula a repetição da velocidade do boss em um reset
                    glutTimerFunc(65,moveBalaBoss,33);
                    glutTimerFunc(20, moveBoss, 33);
                    contadorCena = 1;
                }
            }
            else if(contador == 28){ //Repete a criação do boss para atualização de tela
                criar_boss();
                criar_projeteis_Boss(0);
            }
            //Vitória
            if(vidaBoss == 0){
                glutSwapBuffers();
                contadorReset = 0;
                contadorEspera++;
                continuar = true;
                paused = false;
                contadorEnter = 3;
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Limpa a janela e profundidade
                    glMatrixMode(GL_PROJECTION);
                    glLoadIdentity();
                    glOrtho(-100, 100, -100, 100, -1, 1);
                    glMatrixMode(GL_MODELVIEW);

                    glPushMatrix();
                    glColor3f (0, 0, 0);
                    glBegin(GL_TRIANGLE_FAN);
                        glVertex3f(-100, -100,  0);
                        glVertex3f( 100, -100,  0);
                        glVertex3f( 100,  100,  0);
                        glVertex3f(-100,  100,  0);
                    glEnd();
                    glPopMatrix();

                    glPushMatrix();
                    glTranslatef(0,70,0);
                    glColor3f(1, 1, 1);
                    // Habilita o uso de texturas
                    glEnable(GL_TEXTURE_2D);

                    // Começa a usar a textura que criamos
                    glBindTexture(GL_TEXTURE_2D, idTexturaVitoria1);
                    glBegin(GL_POLYGON);
                        glTexCoord2f(0, 0);
                        glVertex3f(-55, -6, 0);
                        glTexCoord2f(1, 0);
                        glVertex3f(55, -6, 0);
                        glTexCoord2f(1, 1);
                        glVertex3f(55, 6, 0);
                        glTexCoord2f(0, 1);
                        glVertex3f(-55, 6, 0);
                    glEnd();
                    glPopMatrix();
                    glDisable(GL_TEXTURE_2D);

                    glPushMatrix();
                    glTranslatef(0,40,0);
                    glColor3f(1, 1, 1);
                    // Habilita o uso de texturas
                    glEnable(GL_TEXTURE_2D);

                    // Começa a usar a textura que criamos
                    glBindTexture(GL_TEXTURE_2D, idTexturaVitoria2);
                    glBegin(GL_POLYGON);
                        glTexCoord2f(0, 0);
                        glVertex3f(-55, -6, 0);
                        glTexCoord2f(1, 0);
                        glVertex3f(55, -6, 0);
                        glTexCoord2f(1, 1);
                        glVertex3f(55, 6, 0);
                        glTexCoord2f(0, 1);
                        glVertex3f(-55, 6, 0);
                    glEnd();
                    glPopMatrix();
                    glDisable(GL_TEXTURE_2D);

                    glPushMatrix();
                    glTranslatef(0,10,0);
                    glColor3f(1, 1, 1);
                    // Habilita o uso de texturas
                    glEnable(GL_TEXTURE_2D);

                    // Começa a usar a textura que criamos
                    glBindTexture(GL_TEXTURE_2D, idTexturaVitoria3);
                    glBegin(GL_POLYGON);
                        glTexCoord2f(0, 0);
                        glVertex3f(-55, -6, 0);
                        glTexCoord2f(1, 0);
                        glVertex3f(55, -6, 0);
                        glTexCoord2f(1, 1);
                        glVertex3f(55, 6, 0);
                        glTexCoord2f(0, 1);
                        glVertex3f(-55, 6, 0);
                    glEnd();
                    glPopMatrix();
                    glDisable(GL_TEXTURE_2D);

                    glPushMatrix();
                    glTranslatef(0,-20,0);
                    glColor3f(1, 1, 1);
                    // Habilita o uso de texturas
                    glEnable(GL_TEXTURE_2D);

                    // Começa a usar a textura que criamos
                    glBindTexture(GL_TEXTURE_2D, idTexturaVitoria4);
                    glBegin(GL_POLYGON);
                        glTexCoord2f(0, 0);
                        glVertex3f(-55, -6, 0);
                        glTexCoord2f(1, 0);
                        glVertex3f(55, -6, 0);
                        glTexCoord2f(1, 1);
                        glVertex3f(55, 6, 0);
                        glTexCoord2f(0, 1);
                        glVertex3f(-55, 6, 0);
                    glEnd();
                    glPopMatrix();
                    glDisable(GL_TEXTURE_2D);

                    glPushMatrix();
                    glTranslatef(0,-50,0);
                    glColor3f(1, 1, 1);
                    // Habilita o uso de texturas
                    glEnable(GL_TEXTURE_2D);

                    // Começa a usar a textura que criamos
                    glBindTexture(GL_TEXTURE_2D, idTexturaVitoria5);
                    glBegin(GL_POLYGON);
                        glTexCoord2f(0, 0);
                        glVertex3f(-25, -6, 0);
                        glTexCoord2f(1, 0);
                        glVertex3f(25, -6, 0);
                        glTexCoord2f(1, 1);
                        glVertex3f(25, 6, 0);
                        glTexCoord2f(0, 1);
                        glVertex3f(-25, 6, 0);
                    glEnd();
                    glPopMatrix();
                    glDisable(GL_TEXTURE_2D);

                    glPushMatrix();
                    glTranslatef(0,-85,0);
                    glColor3f(1, 1, 1);
                    // Habilita o uso de texturas
                    glEnable(GL_TEXTURE_2D);

                    // Começa a usar a textura que criamos
                    glBindTexture(GL_TEXTURE_2D, idTexturaVitoria6);
                    glBegin(GL_POLYGON);
                        glTexCoord2f(0, 0);
                        glVertex3f(-25, -9, 0);
                        glTexCoord2f(1, 0);
                        glVertex3f(25, -9, 0);
                        glTexCoord2f(1, 1);
                        glVertex3f(25, 9, 0);
                        glTexCoord2f(0, 1);
                        glVertex3f(-25, 9, 0);
                    glEnd();
                    glPopMatrix();
                    glDisable(GL_TEXTURE_2D);

                    glPushMatrix();
                    glTranslatef(80,-95,0);
                    glColor3f(1, 1, 1);
                    // Habilita o uso de texturas
                    glEnable(GL_TEXTURE_2D);

                    // Começa a usar a textura que criamos
                    glBindTexture(GL_TEXTURE_2D, idTexturaPressEnter);
                    glBegin(GL_POLYGON);
                        glTexCoord2f(0, 0);
                        glVertex3f(-15, -3, 0);
                        glTexCoord2f(1, 0);
                        glVertex3f(15, -3, 0);
                        glTexCoord2f(1, 1);
                        glVertex3f(15, 3, 0);
                        glTexCoord2f(0, 1);
                        glVertex3f(-15, 3, 0);
                    glEnd();
                    glPopMatrix();
                    glDisable(GL_TEXTURE_2D);

                    glPushMatrix();
                    glTranslatef(-80,-95,0);
                    glColor3f(1, 1, 1);
                    // Habilita o uso de texturas
                    glEnable(GL_TEXTURE_2D);

                    // Começa a usar a textura que criamos
                    glBindTexture(GL_TEXTURE_2D, idTexturaPressR);
                    glBegin(GL_POLYGON);
                        glTexCoord2f(0, 0);
                        glVertex3f(-15, -3, 0);
                        glTexCoord2f(1, 0);
                        glVertex3f(15, -3, 0);
                        glTexCoord2f(1, 1);
                        glVertex3f(15, 3, 0);
                        glTexCoord2f(0, 1);
                        glVertex3f(-15, 3, 0);
                    glEnd();
                    glPopMatrix();
                    glDisable(GL_TEXTURE_2D);

                    //glutSwapBuffers();
                    //delay(6000);
            }
            glutSwapBuffers();
        }
    }
}

void inicializa() { //Inicializa tudo! Então o boss já existe desde o princípio da partida
    glClearColor(0, 0, 0, 0);
    iniciar_players();
    float posicao = -60;
    for(int i=27; i>18; i--){
        iniciar_inimigo(27-i,posicao,70);
        iniciar_projeteis_inimigos(27-i,posicao+1,70);
        posicao = posicao + 15;
    }
    posicao = -40;
    for(int i=18; i>9; i--){
        iniciar_inimigo(27-i,posicao,55);
        iniciar_projeteis_inimigos(27-i,posicao+1,55);
        posicao = posicao + 15;
    }
    posicao = -60;
    for(int i=9; i>0; i--){
        iniciar_inimigo(27-i,posicao,40);
        iniciar_projeteis_inimigos(27-i,posicao+1,40);
        posicao = posicao + 15;
    }
    iniciar_projeteis_player(-120,0);
    iniciar_projeteis_player(-120,1);

    iniciar_vida(0,-98,-90);
    iniciar_vida(1,-94,-90);
    iniciar_vida(2,-90,-90);

    iniciar_boss(-500,-500);
    iniciar_projeteis_Boss(0,-500,-500);

    // Habilita mesclagem de cores, para termos suporte a texturas com transparência
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Texturas
    idTexturaDoom = carregaTextura("Texturas/fases/doom.jpg");
    idTexturaEspera = carregaTextura("Texturas/fases/espera1.jpeg");
    idTexturaMoldura = carregaTextura("Texturas/fases/moldura1.png");
    idTexturaFundo = carregaTextura("Texturas/tela_inicial/fundo.png");
    idTexturaOp1 = carregaTextura("Texturas/tela_inicial/menu_opcao1.png");
    idTexturaGalaxian = carregaTextura("Texturas/tela_inicial/galaxian.png");
    idTexturaCab = carregaTextura("Texturas/fases/cabecalho.png");
    idTexturaInstrucoes = carregaTextura("Texturas/tela_instrucoes/fundoinstrucoes.png");
    idTexturaPalavrasInstrucoes = carregaTextura("Texturas/tela_instrucoes/a1.png");
    idTexturaInstrucao1 = carregaTextura("Texturas/tela_instrucoes/a2.png");
    idTexturaInstrucao2 = carregaTextura("Texturas/tela_instrucoes/a3.png");
    idTexturaInstrucao3 = carregaTextura("Texturas/tela_instrucoes/a4.png");
    idTexturaInstrucao4 = carregaTextura("Texturas/tela_instrucoes/a5.png");
    idTexturaInstrucao5 = carregaTextura("Texturas/tela_instrucoes/a6.png");
    idTexturaPress = carregaTextura("Texturas/tela_instrucoes/press.png");
    idTexturaPressEnter = carregaTextura("Texturas/tela_derrota/enter-to-exit.png");
    idTexturaPressR = carregaTextura("Texturas/tela_derrota/r-to-restart.png");
    idTexturaDerrota1 = carregaTextura("Texturas/tela_derrota/derrota1.png");
    idTexturaDerrota2 = carregaTextura("Texturas/tela_derrota/derrota2.png");
    idTexturaDerrota3 = carregaTextura("Texturas/tela_derrota/derrota3.png");
    idTexturaVitoria1 = carregaTextura("Texturas/tela_vitoria/vitoria1.png");
    idTexturaVitoria2 = carregaTextura("Texturas/tela_vitoria/vitoria2.png");
    idTexturaVitoria3 = carregaTextura("Texturas/tela_vitoria/vitoria3.png");
    idTexturaVitoria4 = carregaTextura("Texturas/tela_vitoria/vitoria4.png");
    idTexturaVitoria5 = carregaTextura("Texturas/tela_vitoria/vitoria5.png");
    idTexturaVitoria6 = carregaTextura("Texturas/tela_vitoria/vitoria6.png");
    idTexturaPause = carregaTextura("Texturas/telas/paused.png");
    idTexturaWarning = carregaTextura("Texturas/telas/warning.png");
}

void deteccaoTeclado(int chave, int x, int y){ //Teclado GLUT
    if(!paused && setas == true){
        if(chave == GLUT_KEY_LEFT){ // Esquerda
            if(pegar_xesq() > -88){ // Limite de tela
                setar_xesq(-4);
            }
        }
        else if(chave == GLUT_KEY_RIGHT){ // Direita
            if(pegar_xdir() < 80){ // Limite de tela
                setar_xdir(4);
            }
        }
        glutPostRedisplay();
    }
}

void ascii(unsigned char key, int x, int y){ //Teclado especial char GLUT
    switch(key) {
    case 27:      // Tecla "ESC"
        exit(0);  // O jogo é encerrado
        break;
    case 32:      // Tecla "Espaço"
        if(!paused && setas == true){
            if(posicaoY_bala_player(0) == -80){ // Impede tiros seguidos
                iniciar_projeteis_player(pegar_xdir()+3,0);
                glutTimerFunc(75, moveBalaPlayer, 60); // Atira
            }
            else if(posicaoY_bala_player(1) == -80){ // Impede tiros seguidos
                iniciar_projeteis_player(pegar_xdir()+3,1);
                glutTimerFunc(75, moveBalaPlayer1, 60); // Atira
            }
        };
        break;
    case 112:     // Tecla "p"
    case 80:      // Shift+p, CAPSLOCK
        if(continuar == true){
            if(paused == true){
                paused = false;
                iniciar_pause(-200,-200);
                criar_pause();
                glutSwapBuffers();
                glutTimerFunc(25, moveInimigo, 33);
                glutTimerFunc(50, moveBalaInimiga1, 60); // Dá aleatoriedade ao sistema (50,75)
                glutTimerFunc(75, moveBalaInimiga2, 60); // Dá aleatoriedade ao sistema (50,75)
                glutTimerFunc(75, moveBalaPlayer, 60);
                glutTimerFunc(75, moveBalaPlayer1, 60);
                glutTimerFunc(20, moveBoss, 33);
                glutTimerFunc(45,moveBalaBoss,33);
            }
            else if(paused == false){
                paused = true;
                iniciar_pause(-25,-10);
                criar_pause();
                glutSwapBuffers();
            }
        };
        break;
    case 114:     // Tecla "r"
    case 82:      // Shift+r, CAPSLOCK
        if(continuar == true && paused == false && contadorReset == 0){
            contadorEspera = 0;
            inicializa();
            contador = 0;
            life = 3;
            vidaBoss = 15;
            glutDisplayFunc(desenhaMinhaCena);
        }
        break;
    case 13: // ENTER
        if(contadorEnter == 0){
            glutDisplayFunc(desenhaTelaInstrucoes);
            glutIdleFunc(atualiza);
            contadorEnter = 1;
        }
        else if(contadorEnter == 1){
            contadorEnter = 2;
            glutDisplayFunc(desenhaMinhaCena);
            glutIdleFunc(atualiza);
            // Ativa o automático do inimigo
            glutTimerFunc(25, moveInimigo, 33);
            glutTimerFunc(50, moveBalaInimiga1, 60); // Dá aleatoriedade ao sistema (50,75)
            glutTimerFunc(75, moveBalaInimiga2, 60); // Dá aleatoriedade ao sistema (50,75)
        }
        else if(contadorEnter == 3){
            exit(0);
        }
        break;
    default:
        break;
    }
}

//Movimentar 8 inimigos passo a passo - Jogo da cobrinha
/*
direcao_padrao_inimigo();
void moveInimigo(int local){
    for (int i = 0; i<8; i++){
        if(pegar_xdir_inimigo(i) <= -88 && retornaDirecao(i) == false){
            troca_direcao_inimigo(retornaDirecao(i), i);
            setar_ybaixo(-20,i);
        }
        else if(pegar_xdir_inimigo(i) >= 88 && retornaDirecao(i) == true){
            troca_direcao_inimigo(retornaDirecao(i), i);
            setar_ybaixo(-20,i);
        }
        else if(retornaDirecao(i) == false){
            setar_xesq_inimigo(-8,i);
        }
        else if(retornaDirecao(i) == true){
            setar_xdir_inimigo(8,i);
        }
    }
    glutPostRedisplay();
    glutTimerFunc(1000, moveInimigo, 33);
}
*/

void moveBoss(int c){
    if(!paused){
        if(pegarBoss_xesq() <= -88 && direcao_boss == true){ // Limite de tela
            direcao_boss = false;
        }
        else if(pegarBoss_xdir() >= 60 && direcao_boss == false){ // Limite de tela
            direcao_boss = true;
        }
        else if(direcao_boss == true){
            setarBoss_xesq(-0.9); // 5 tiros
        }
        else if(direcao_boss == false){
            setarBoss_xdir(0.9); // 5 tiros
        }
        glutPostRedisplay();
        glutTimerFunc(20, moveBoss, 33);
    }
}

//Movimentação em blocos
void moveInimigo(int local){
    if(!paused){
        if(mudaDirecao == false){
            for(int y = 0; y<27; y++){
                if(pegar_ybaixo(y) <= 100 && pegar_ybaixo(y)>= -100){
                    setar_xesq_inimigo(-0.2,y);
                }
            }
            for(int y = 0; y<27; y++){
                if(pegar_ybaixo(y) <= 100 && pegar_ybaixo(y)>= -100){
                    if(pegar_x_inimigo(y) <= -88){ // Quando o primeiro elemento tocar essa posição ocorrerá uma descida
                        setar_ybaixoALL(-10);
                        mudaDirecao = true;
                    }
                }
            }
        }
        else if(mudaDirecao == true){
            for(int y = 0; y<27; y++){
                if(pegar_ybaixo(y) <= 100 && pegar_ybaixo(y)>= -100){
                    setar_xdir_inimigo(0.2,y);
                }
            }
            for(int y = 0; y<27; y++){
                if(pegar_ybaixo(y) <= 100 && pegar_ybaixo(y)>= -100){
                    if(pegar_x_inimigo(y) >= 82){
                        setar_ybaixoALL(-10);
                        mudaDirecao = false;
                    }
                }
            }
        }

        glutPostRedisplay();
        glutTimerFunc(25, moveInimigo, 33);
    }
}

void moveBalaInimiga1(int b){ //Conjunto de projéteis 1
    if(!paused){
        int vetor1[5] = {13,23,25,1,15};
        for (int i = 0; i<5; i++){ //Análise da colisão
            if(colisaoBalaInimigo(bala_inimigo[vetor1[i]].x,posicaoY_bala_inimigo(vetor1[i]),bala_inimigo[vetor1[i]].comprimento,bala_inimigo[vetor1[i]].altura,pegar_xdir(),player.y,player.altura, player.comprimento) == true){
                setar_projeteis_inimigo(200,vetor1[i]);
                iniciar_projeteis_inimigos(vetor1[i],pegar_x_inimigo(vetor1[i]),pegar_ybaixo(vetor1[i]));
                life--;
                iniciar_vida(life,-200,0);
            }
        }

        for(int i = 0; i<5 ; i++){
            setar_projeteis_inimigo(-5,vetor1[i]);
            for (int y = 0; y<5; y++){
                if(posicaoY_bala_inimigo(vetor1[y]) == -100){ //reseta no limite da tela
                    iniciar_projeteis_inimigos(vetor1[y],pegar_x_inimigo(vetor1[y]),pegar_ybaixo(vetor1[y]));
                }
            }
        }
        glutPostRedisplay();
        glutTimerFunc(50, moveBalaInimiga1, 60);
    }
}

void moveBalaInimiga2(int a){ //Conjunto de projéteis 2
    if(!paused){
        int vetor1[5] = {3,4,8,9,20};
        for (int i = 0; i<5; i++){
            if(colisaoBalaInimigo(bala_inimigo[vetor1[i]].x,posicaoY_bala_inimigo(vetor1[i]),bala_inimigo[vetor1[i]].comprimento,bala_inimigo[vetor1[i]].altura,pegar_xdir(),player.y,player.altura, player.comprimento) == true){
                setar_projeteis_inimigo(200,vetor1[i]);
                iniciar_projeteis_inimigos(vetor1[i],pegar_x_inimigo(vetor1[i]),pegar_ybaixo(vetor1[i]));
                life--;
                iniciar_vida(life,-200,0);
            }
        }
        for(int i = 0; i<5 ; i++){
            setar_projeteis_inimigo(-5,vetor1[i]);
            for (int y = 0; y<5; y++){
                if(posicaoY_bala_inimigo(vetor1[y]) == -100){
                    iniciar_projeteis_inimigos(vetor1[y],pegar_x_inimigo(vetor1[y]),pegar_ybaixo(vetor1[y]));
                }
            }
        }
        glutPostRedisplay();
        glutTimerFunc(75, moveBalaInimiga2, 60);
    }
}

void moveBalaPlayer(int c){ //Lógica inversa da bala inimiga, então ela sobe, e ela não reinicia pois o tiro não é automático, dessa forma, quando atravessa o limite da tela a bala é iniciada novamente mas apenas criada quando se aperta espaço
    if(!paused){
        for (int i = 0; i<27; i++){
            if(colisaoBalaPlayer(posicaoX_bala_player(0),posicaoY_bala_player(0),bala_player[0].comprimento,bala_player[0].altura,pegar_x_inimigo(i),pegar_ybaixo(i),inimigo[i].altura, inimigo[i].comprimento) == true){
                setar_xdir_inimigo(0, i);
                setar_ybaixo(-900, i);
                contador++;
                setar_projeteis_player(210, 0);
            }
        }
        if(colisaoBalaPlayer(posicaoX_bala_player(0),posicaoY_bala_player(0),bala_player[0].comprimento,bala_player[0].altura,pegarBoss_xdir(),pegar_ybaixoBoss(),chefe.altura, chefe.comprimento) == true){
            vidaBoss--;
            setar_projeteis_player(210, 0);
        }
        setar_projeteis_player(10,0);
        if(posicaoY_bala_player(0) <= 100){ // Repete até que a bala do player chegue ao ponto máximo da tela
            glutPostRedisplay();
            glutTimerFunc(75, moveBalaPlayer, 60);
        }
        else{
           iniciar_projeteis_player(-120,0);
        }
    }
}

void moveBalaPlayer1(int d){
    if(!paused){
        for (int i = 0; i<27; i++){
            if(colisaoBalaPlayer(posicaoX_bala_player(1),posicaoY_bala_player(1),bala_player[1].comprimento,bala_player[1].altura,pegar_x_inimigo(i),pegar_ybaixo(i),inimigo[i].altura, inimigo[i].comprimento) == true){
                setar_xdir_inimigo(0, i);
                setar_ybaixo(-900, i);
                contador++;
                setar_projeteis_player(210, 1);
            }
        }
        if(colisaoBalaPlayer(posicaoX_bala_player(1),posicaoY_bala_player(1),bala_player[1].comprimento,bala_player[1].altura,pegarBoss_xdir(),pegar_ybaixoBoss(),chefe.altura, chefe.comprimento) == true){
            vidaBoss--;
            setar_projeteis_player(210, 1);
        }
        setar_projeteis_player(10,1);
        if(posicaoY_bala_player(1) <= 100){ // Repete até que a bala do player chegue ao ponto máximo da tela
            glutPostRedisplay();
            glutTimerFunc(75, moveBalaPlayer1, 60);
        }
        else{
           iniciar_projeteis_player(-120,1);
        }
    }
}

void moveBalaBoss(int a){ //Mesma lógica da bala inimiga porém com apenas um projétil maior
    if(!paused){
        if(colisaoBalaInimigo(bala_Boss[0].x,posicaoY_bala_Boss(0),bala_Boss[0].comprimento,bala_inimigo[0].altura,pegar_xdir(),player.y,player.altura, player.comprimento) == true){
            setar_projeteis_Boss(200,0);
            iniciar_projeteis_Boss(0,chefe.x+10,chefe.y);
            life--;
            iniciar_vida(life,-200,0);
        }
        setar_projeteis_Boss(-10,0);
        if(posicaoY_bala_Boss(0) >= -110){ // Repete até que a bala do player chegue ao ponto máximo da tela
            glutPostRedisplay();
            glutTimerFunc(65,moveBalaBoss,33);
        }
        else{
           iniciar_projeteis_Boss(0,chefe.x+10,chefe.y);
           glutTimerFunc(65,moveBalaBoss,33);
        }
        glutPostRedisplay();
    }
}

void delay(int milliseconds){
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

//Colisões
bool colisao(float inimX,float inimY,float inimCOMP, float inimALT, float playerX, float playerY, float playerALT, float playerCOMP){
    if(inimY+3 > playerY+playerALT){
        return false;
    }
    else if(inimX+inimCOMP-3 < playerX){
        return false;
    }
    else if(inimX+1 > playerX+playerCOMP){
        return false;
    }
    else if(playerY > inimY+inimALT-3){
        return false;
    }
    return true;
}

bool colisaoBalaPlayer(float playerBX,float playerBY,float playerBCOMP, float playerBALT, float inimX, float inimY, float inimALT, float inimCOMP){
    if(playerBY > inimY+inimALT){
        return false;
    }
    else if(playerBX+playerBCOMP < inimX){
        return false;
    }
    else if(playerBX > inimX+inimCOMP){
        return false;
    }
    else if(playerBALT+playerBY < inimY){
        return false;
    }
    return true;
}

bool colisaoBalaInimigo(float inimBX,float inimBY,float inimBCOMP, float inimBALT, float playerX, float playerY, float playerALT, float playerCOMP){
    if(inimBY+2 > playerY+playerALT){
        return false;
    }
    else if(inimBX+inimBCOMP-2 < playerX){
        return false;
    }
    else if(inimBX+0.5 > playerX+playerCOMP){
        return false;
    }
    else if(inimBALT+inimBY-2 < playerY){
        return false;
    }
    return true;
}
