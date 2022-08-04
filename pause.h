#include <stdio.h>
#include <SOIL/SOIL.h>

GLuint idTexturaPause;

typedef struct InfPause{
	float x,y;
	float comprimento,altura;
	bool direcao;
}Pause;

Pause p;

void iniciar_pause(float pos, float dist){
    p.x = pos;
    p.y = dist;
    p.comprimento = 50;
    p.altura = 20;

    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void criar_pause(){
    glPushMatrix();
    glColor3f (1, 1, 1);

    //Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, idTexturaPause);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(p.x, p.y, 0);
        glTexCoord2f(1, 0);
        glVertex3f(p.x+p.comprimento, p.y, 0);
        glTexCoord2f(1, 1);
        glVertex3f(p.x+p.comprimento, p.y+p.altura, 0);
        glTexCoord2f(0, 1);
        glVertex3f(p.x, p.y+p.altura, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

