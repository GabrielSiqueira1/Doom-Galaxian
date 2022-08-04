#include <SOIL/SOIL.h>

//Texturas Gerais
GLuint idTexturaGalaxian;
GLuint idTexturaDoom;
GLuint idTexturaFundo;
GLuint idTexturaOp1;
GLuint idTexturaCab;
GLuint idTexturaMoldura;
GLuint idTexturaInstrucoes;
GLuint idTexturaPalavrasInstrucoes;
GLuint idTexturaPress;
GLuint idTexturaPressR;
GLuint idTexturaPressEnter;
GLuint idTexturaEspera;
GLuint idTexturaInstrucao1;
GLuint idTexturaInstrucao2;
GLuint idTexturaInstrucao3;
GLuint idTexturaInstrucao4;
GLuint idTexturaInstrucao5;
GLuint idTexturaDerrota1;
GLuint idTexturaDerrota2;
GLuint idTexturaDerrota3;
GLuint idTexturaVitoria1;
GLuint idTexturaVitoria2;
GLuint idTexturaVitoria3;
GLuint idTexturaVitoria4;
GLuint idTexturaVitoria5;
GLuint idTexturaVitoria6;
GLuint idTexturaWarning;

GLuint carregaTextura(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(arquivo,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

    if (idTextura == 0) { // Caso a textura seja inalcançável
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}
