#include <stdio.h>
#include <SOIL/SOIL.h>

GLuint idTexturaVida;
GLuint carregaTexturaVida(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(arquivo,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

    if (idTextura == 0) { // Caso a textura seja inalcançável
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}

typedef struct InfVida{
	float x,y;
	float comprimento,altura;
	bool direcao;
}Vida;

Vida vida[30]; //konami code

void iniciar_vida(int i, float pos, float dist){
    vida[i].x = pos;
    vida[i].y = dist;
    vida[i].comprimento = 3.5;
    vida[i].altura = 7;

    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    idTexturaVida = carregaTexturaVida("Texturas/player/vida.png");

}

void criar_vida(int i){
    glPushMatrix();
    glColor3f (1, 1, 1);

    //Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, idTexturaVida);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(vida[i].x, vida[i].y, 0);
        glTexCoord2f(1, 0);
        glVertex3f(vida[i].x+vida[i].comprimento, vida[i].y, 0);
        glTexCoord2f(1, 1);
        glVertex3f(vida[i].x+vida[i].comprimento, vida[i].y+vida[i].altura, 0);
        glTexCoord2f(0, 1);
        glVertex3f(vida[i].x, vida[i].y+vida[i].altura, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
