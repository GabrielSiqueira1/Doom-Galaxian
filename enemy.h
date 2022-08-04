#include <stdio.h>
#include <SOIL/SOIL.h>

GLuint idTexturaEnemy;
// Coloca a textura no fundo da tela
GLuint carregaTexturaEnemy(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(arquivo,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

    if (idTextura == 0) { // Caso a textura seja inalcançável
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}

typedef struct InfEnemy{
	float x,y;
	float comprimento,altura;
	bool direcao;
}Enemy;

Enemy inimigo[27];

void iniciar_inimigo(int i, float pos, float dist){
    inimigo[i].x = pos;
    inimigo[i].y = dist;
    inimigo[i].comprimento = 6;
    inimigo[i].altura = 10;

    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    idTexturaEnemy = carregaTexturaEnemy("Texturas/inimigo/amoeba.png");

}

void criar_inimigo(int i){
    glPushMatrix();
    glColor3f (1, 1, 1);

    //Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, idTexturaEnemy);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(inimigo[i].x, inimigo[i].y, 0);
        glTexCoord2f(1, 0);
        glVertex3f(inimigo[i].x+inimigo[i].comprimento, inimigo[i].y, 0);
        glTexCoord2f(1, 1);
        glVertex3f(inimigo[i].x+inimigo[i].comprimento, inimigo[i].y+inimigo[i].altura, 0);
        glTexCoord2f(0, 1);
        glVertex3f(inimigo[i].x, inimigo[i].y+inimigo[i].altura, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

// Movimentação em bloco
void setar_x_inimigoALL(float moverX_inimigo){
    for (int i = 0; i<27; i++){
        inimigo[i].x += moverX_inimigo;
    }
}

// Descendo em bloco
void setar_ybaixoALL(float moverYbaixo){
    for (int i=0; i<27; i++){
        inimigo[i].y += moverYbaixo;
    }
}

// Análise única - Movimentação separada, à esquerda
void setar_xesq_inimigo(float moverXesq_inimigo, int i){
    inimigo[i].x += moverXesq_inimigo;
}

// Análise única - Movimentação separada, à direita
void setar_xdir_inimigo(float moverXdir_inimigo, int i){
    inimigo[i].x += moverXdir_inimigo;
}

//Descida Única
void setar_ybaixo(float moverYbaixo, int i){
    inimigo[i].y += moverYbaixo;
}

float pegar_ybaixo(int i){
    return inimigo[i].y;
}

float pegar_x_inimigo(int i){
    return inimigo[i].x;
}

//Alteração de direções - em linha
/*
void direcao_padrao_inimigo(){
    for (int i = 0; i<9; i++){
        inimigo[i].direcao = false;
    }
}

bool troca_direcao_inimigo(bool reboco, int i){
    if(reboco == true){
        inimigo[i].direcao = false;
    }
    else if (reboco == false){
        inimigo[i].direcao = true;
    }
    return inimigo[i].direcao;
}

bool retornaDirecao(int i){
    return inimigo[i].direcao;
}

void troca_direcao_inimigoALL(bool reboco){
    if(reboco == true){
        for (int i=0; i<9; i++){
            inimigo[i].direcao = false;
        }
    }
    else if (reboco == false){
        for (int i=0; i<9; i++){
            inimigo[i].direcao = true;
        }
    }
}
*/
