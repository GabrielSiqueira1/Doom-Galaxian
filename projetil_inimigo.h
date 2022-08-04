#include <stdio.h>
#include <SOIL/SOIL.h>

GLuint idTexturaBalaInimigo;
// Coloca a textura no fundo da tela
GLuint carregaTexturaBalaInimigo(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(arquivo,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

    if (idTextura == 0) { // Caso a textura seja inalcançável
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}
typedef struct InfProjetil_Inimigo{
	float x,y;
	float comprimento,altura;
}PI;

PI bala_inimigo[27];

void iniciar_projeteis_inimigos(int i, float pos, float dist){
    bala_inimigo[i].x = pos;
    bala_inimigo[i].y = dist;
    bala_inimigo[i].comprimento = 3;
    bala_inimigo[i].altura = 6;
    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    idTexturaBalaInimigo = carregaTexturaBalaInimigo("Texturas/inimigo/balinha_inimiga.png");
}

void criar_projeteis_inimigos(int i){
    glPushMatrix();
    glColor3f(1, 1, 1);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, idTexturaBalaInimigo);

    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(bala_inimigo[i].x, bala_inimigo[i].y, 0);
        glTexCoord2f(1, 0);
        glVertex3f(bala_inimigo[i].x+bala_inimigo[i].comprimento, bala_inimigo[i].y, 0);
        glTexCoord2f(1, 1);
        glVertex3f(bala_inimigo[i].x+bala_inimigo[i].comprimento, bala_inimigo[i].y+bala_inimigo[i].altura, 0);
        glTexCoord2f(0, 1);
        glVertex3f(bala_inimigo[i].x, bala_inimigo[i].y+bala_inimigo[i].altura, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void setar_projeteis_inimigo(float moverYprojetil, int i){
    bala_inimigo[i].y += moverYprojetil;
}

float posicaoY_bala_inimigo(int i){
    return bala_inimigo[i].y;
}
