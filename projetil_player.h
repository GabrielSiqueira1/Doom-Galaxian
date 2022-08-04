#include <stdio.h>
#include <SOIL/SOIL.h>

GLuint idTexturaBalaPlayer;
// Coloca a textura no fundo da tela
GLuint carregaTexturaBalaPlayer(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(arquivo,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

    if (idTextura == 0) { // Caso a textura seja inalcançável
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}
typedef struct InfProjetil_Player{
	float x,y;
	float comprimento,altura;
}PP;

PP bala_player[20];

void iniciar_projeteis_player(float pos, int i){
    bala_player[i].x = pos;
    bala_player[i].y = -80;
    bala_player[i].comprimento = 2;
    bala_player[i].altura = 4;
    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    idTexturaBalaPlayer = carregaTexturaBalaPlayer("Texturas/player/balinha.png");
}

void criar_projeteis_player(int i){
    glPushMatrix();
    glColor3f(1, 1, 1);

    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaBalaPlayer);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(bala_player[i].x, bala_player[i].y, 0);
        glTexCoord2f(1, 0);
        glVertex3f(bala_player[i].x+bala_player[i].comprimento, bala_player[i].y, 0);
        glTexCoord2f(1, 1);
        glVertex3f(bala_player[i].x+bala_player[i].comprimento, bala_player[i].y+bala_player[i].altura, 0);
        glTexCoord2f(0, 1);
        glVertex3f(bala_player[i].x,bala_player[i].y+bala_player[i].altura, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void setar_projeteis_player(float moverYprojetil, int i){
    bala_player[i].y += moverYprojetil;
}

float posicaoY_bala_player(int i){
    return bala_player[i].y;
}

float posicaoX_bala_player(int i){
    return bala_player[i].x;
}
