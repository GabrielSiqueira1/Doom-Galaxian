#include <stdio.h>
#include <SOIL/SOIL.h>

GLuint idTexturaBalaBoss;
// Coloca a textura no fundo da tela
GLuint carregaTexturaBalaBoss(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(arquivo,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

    if (idTextura == 0) { // Caso a textura seja inalcançável
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}
typedef struct InfProjetil_Boss{
	float x,y;
	float comprimento,altura;
}PB;

PB bala_Boss[10];

void iniciar_projeteis_Boss(int i, float pos, float dist){
    bala_Boss[i].x = pos;
    bala_Boss[i].y = dist;
    bala_Boss[i].comprimento = 15;
    bala_Boss[i].altura = 20;
    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    idTexturaBalaBoss = carregaTexturaBalaBoss("Texturas/inimigo/balinha_inimiga.png");
}

void criar_projeteis_Boss(int i){
    glPushMatrix();
    glColor3f(1, 1, 1);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, idTexturaBalaBoss);

    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(bala_Boss[i].x, bala_Boss[i].y, 0);
        glTexCoord2f(1, 0);
        glVertex3f(bala_Boss[i].x+bala_Boss[i].comprimento, bala_Boss[i].y, 0);
        glTexCoord2f(1, 1);
        glVertex3f(bala_Boss[i].x+bala_Boss[i].comprimento, bala_Boss[i].y+bala_Boss[i].altura, 0);
        glTexCoord2f(0, 1);
        glVertex3f(bala_Boss[i].x, bala_Boss[i].y+bala_Boss[i].altura, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void setar_projeteis_Boss(float moverYprojetil, int i){
    bala_Boss[i].y += moverYprojetil;
}

float posicaoY_bala_Boss(int i){
    return bala_Boss[i].y;
}

