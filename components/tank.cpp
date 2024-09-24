// tank.cpp
#include "tank.h"
#include <GL/glut.h> // Certifique-se de incluir GLUT para usar as funções de OpenGL
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h" 

GLuint textureID;

// Função para configurar a textura usando stb_image para carregar PNGs
void loadTexture(const char* filename) {
    int width, height, nrChannels;
    
    // Carregar a imagem com stb_image
    unsigned char* image = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (image == nullptr) {
        printf("Erro ao carregar a textura: %s\n", filename);
        return;
    }

    // Gerar um identificador para a textura
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Dependendo do número de canais (RGB ou RGBA), configure a textura
    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

    // Carregar a imagem como textura
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);

    // Definir parâmetros de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Liberar a memória alocada pela stb_image
    stbi_image_free(image);
}

void drawTank(float tankPosX, float tankPosZ, float angleYY, float angleY) {

    loadTexture("texture.png");
    // Desenhar o tanque
    glPushMatrix();

        GLUquadric* quad = gluNewQuadric();

        // Aplicar translação com base na posição do tanque
        glTranslatef(tankPosX, 0.0, tankPosZ);
        glRotatef(angleYY, 0.0f, 1.0f, 0.0f);

        // Habilitar texturas
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID); // Vincular a textura carregada

        // Corpo do tanque com textura
        glPushMatrix();
            glScalef(3.0, 1.0, 2.0);
            glColor3f(1.0, 1.0, 1.0); // Branco para não interferir na textura

            // Ativar coordenadas de textura para o corpo do tanque
            glBegin(GL_QUADS);
                // Frente
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);

                // Traseira
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);

                // Lado Esquerdo
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);

                // Lado Direito
                glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);

                // Topo
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);

                // Base
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.5f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
            glEnd();
        glPopMatrix();

        // Desabilitar texturas para o restante do tanque
        glDisable(GL_TEXTURE_2D);

        // Frente (sem textura)
        glPushMatrix();
            glScalef(0.5f, 0.5f, 1.8f);
            glTranslatef(-3.0f, 0, 0);
            glColor3f(0.3, 0.3, 0.3);
            glutSolidCube(1.0);
        glPopMatrix();

        // Torre e Cano (sem textura)
        glPushMatrix();
            glRotatef(angleY, 0.0f, 1.0f, 0.0f);
            glTranslatef(0.0, 0.75, 0.0);
            glScalef(1.5, 1.0, 1.5);
            glColor3f(0.0, 0.5, 0.0);
            glutSolidCube(1.0);

            glPushMatrix();
                glTranslatef(0.75, 0.0, 0.0);
                glScalef(2.0, 0.3, 0.3);
                glColor3f(0.5, 0.5, 0.5);
                glutSolidCube(1.0);
            glPopMatrix();

        glPopMatrix();

        // Rodas (sem textura)
        for (int i = -1; i <= 1; i += 2) {
            glPushMatrix();
                glTranslatef(0.0, -0.5, i * 1.25);
                glScalef(3.0, 0.5, 0.5);
                glColor3f(0.3, 0.3, 0.3);
                glutSolidCube(1.0);
            glPopMatrix();
        }

    gluDeleteQuadric(quad);
    glPopMatrix();
}
