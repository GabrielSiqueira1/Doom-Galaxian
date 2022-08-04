#include <stdio.h>
#include <SOIL/SOIL.h>

GLuint idTexturaBoss;
// Coloca a textura no fundo da tela
GLuint carregaTexturaBoss(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(arquivo,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

    if (idTextura == 0) { // Caso a textura seja inalcançável
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}

typedef struct InfBoss{
	float x,y;
	float comprimento,altura;
}Boss;

Boss chefe;

void iniciar_boss(float pos, float dist){
    chefe.x = pos;
    chefe.y = dist;
    chefe.comprimento = 30;
    chefe.altura = 55;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    idTexturaBoss = carregaTexturaBoss("Texturas/inimigo/boss.png");
}

void criar_boss(){
    glPushMatrix();
    glColor3f (1, 1, 1);

    //Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaBoss);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(chefe.x, chefe.y, 0);
        glTexCoord2f(1, 0);
        glVertex3f(chefe.x+chefe.comprimento, chefe.y, 0);
        glTexCoord2f(1, 1);
        glVertex3f(chefe.x+chefe.comprimento, chefe.y+chefe.altura, 0);
        glTexCoord2f(0, 1);
        glVertex3f(chefe.x, chefe.y+chefe.altura, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //glutSwapBuffers();
}

void setarBoss_xesq(float moverXesq){
    chefe.x += moverXesq;
}

float pegarBoss_xesq(){
    return chefe.x;
}

void setarBoss_xdir(float moverXdir){
    chefe.x += moverXdir;
}

float pegarBoss_xdir(){
    return chefe.x;
}

void setar_ybaixoBoss(float moverYbaixo){
    chefe.y += moverYbaixo;
}

float pegar_ybaixoBoss(){
    return chefe.y;
}

