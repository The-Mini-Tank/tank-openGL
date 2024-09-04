// tank.cpp
#include "tank.h"
#include <GL/glut.h> // Certifique-se de incluir GLUT para usar as funções de OpenGL
#include <GL/glu.h>

void drawTank(float tankPosX, float tankPosZ, float angleYY, float angleY) {

   
    // Desenhar o tanque
    glPushMatrix();

        GLUquadric* quad = gluNewQuadric();

        // Aplicar translação com base na posição do tanque
        glTranslatef(tankPosX, 0.0, tankPosZ);
        glRotatef(angleYY, 0.0f, 1.0f, 0.0f);

        // Corpo
        glPushMatrix();
            glScalef(3.0, 1.0, 2.0);
            glColor3f(0.0, 0.8, 0.0);
            glutSolidCube(1.0);
        glPopMatrix();

        // Torre e Cano
        glPushMatrix();
            glRotatef(angleY, 0.0f, 1.0f, 0.0f); 
            // glRotatef(90, 0.0f, 0.0f, 0.0f); 
            glTranslatef(0.0, 0.75, 0.0); 
            glScalef(1.5, 1.0, 1.5);
            glColor3f(0.0, 0.5, 0.0);
            // gluCylinder(quad, 1.0, 1.0, 3.0, 16, 1);  // (quad, raio da base, raio do topo, altura, slices, stacks)
            glutSolidCube(1.0);

            glPushMatrix();
                glTranslatef(0.75, 0.0, 0.0);
                glScalef(2.0, 0.3, 0.3);
                glColor3f(0.5, 0.5, 0.5);
                glutSolidCube(1.0);
            glPopMatrix();

        glPopMatrix();

        // Rodas
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
