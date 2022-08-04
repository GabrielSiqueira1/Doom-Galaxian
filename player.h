#include <stdio.h>
#include <SOIL/SOIL.h>

GLuint idTexturaPlayer;
// Coloca a textura no fundo da tela
GLuint carregaTexturaPlayer(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(arquivo,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

    if (idTextura == 0) { // Caso a textura seja inalcançável
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}

typedef struct InfPlayer{
	float x,y;
	float comprimento,altura;
}Player;

Player player;

void iniciar_players(){
    player.x = -4;
    player.y = -80;
    player.comprimento = 8;
    player.altura = 16;
    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    idTexturaPlayer = carregaTexturaPlayer("Texturas/player/player.png");
}

void criar_player(){
    glPushMatrix();
    glColor3f (1, 1, 1);

    //Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaPlayer);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(player.x, player.y, 0);
        glTexCoord2f(1, 0);
        glVertex3f(player.x+player.comprimento, player.y, 0);
        glTexCoord2f(1, 1);
        glVertex3f(player.x+player.comprimento, player.y+player.altura, 0);
        glTexCoord2f(0, 1);
        glVertex3f(player.x, player.y+player.altura, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //glutSwapBuffers();
}

void setar_xesq(float moverXesq){
    player.x += moverXesq;
}

float pegar_xesq(){
    return player.x;
}

void setar_xdir(float moverXdir){
    player.x += moverXdir;
}

float pegar_xdir(){
    return player.x;
}
