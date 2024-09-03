#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <cmath>
#include <cstdio>
#include <iostream>

float angleY = 0.0f; // Ângulo de rotação no eixo Y da torre
float angleYY = -90.0f; // Ângulo de rotação no eixo Y da torre
int mouseX, mouseY;  // Posições do mouse
float tankPosX = 0.0f, tankPosZ = 0.0f; // Posição do tanque no plano XZ
float speed = 0.1f; // Velocidade de movimento do tanque
float rotationSpeed = 2.0f; // Velocidade de rotação do tanque


// EXIBIR TEXTO NA TELA 
void renderBitmapString(float x, float y, void *font, const char *string) {
    const char *c;
    glRasterPos2f(x, y); // Define a posição inicial do texto
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void renderTextInPixels(int x, int y, void *font, const char *string) {
    // Salvar as configurações de projeção e modelagem
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Configurar projeção ortográfica baseada nos pixels da janela
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

    // Mudar para a matriz de modelagem
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Posicionar o texto em pixels
    glRasterPos2i(x, y);

    // Renderizar o texto
    renderBitmapString(0, 0, font, string);

    // Restaurar as matrizes de projeção e modelagem
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Texto na tela
    glColor3f(1.0, 1.0, 1.0);
    char buffer[50];
    sprintf(buffer, "Valor: %d", angleY);
    renderTextInPixels(10, glutGet(GLUT_WINDOW_HEIGHT) - 20, GLUT_BITMAP_HELVETICA_18, buffer);
    //

    // Configuração da projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 0.1, 100.0);
    glTranslatef(0, -5, -10);
    glRotatef(45, 1, 0, 0.0f);

    // Configuração da câmera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Desenhar o tanque
    glPushMatrix();
        // Aplicar translação com base na posição do tanque
        glTranslatef(tankPosX, 0.0, tankPosZ);

        // Rotação inicial do tanque (se necessário)
        glRotatef(angleYY, 0.0f, 1.0f, 0.0f);
        
        // Corpo
        glPushMatrix();
            glScalef(3.0, 1.0, 2.0);
            glColor3f(0.0, 0.8, 0.0);
            glutSolidCube(1.0);
        glPopMatrix();

        // Torre e Cano
        glPushMatrix();
            glRotatef(angleY, 0.0f, 1.0f, 0.0f); // Aplica a rotação da torre
            glTranslatef(0.0, 0.75, 0.0); // Translada depois da rotação
            glScalef(1.5, 1.0, 1.5);
            glColor3f(0.0, 0.5, 0.0);
            glutSolidCube(1.0);

            // Cano do tanque (aplicando a rotação já feita)
            glPushMatrix();
                glTranslatef(0.75, 0.0, 0.0); // Translada o cano em relação ao corpo
                glScalef(2.0, 0.3, 0.3);
                glColor3f(0.5, 0.5, 0.5);
                glutSolidCube(1.0);
            glPopMatrix();

        glPopMatrix();

        // Rodas
        for (int i = -1; i <= 1; i += 2) {  // Para desenhar as rodas dos dois lados
            glPushMatrix();
                glTranslatef(0.0, -0.5, i * 1.25); // Posicionando as rodas lateralmente
                glScalef(3.0, 0.5, 0.5); // Alongando as rodas no eixo X
                glColor3f(0.3, 0.3, 0.3);
                glutSolidCube(1.0);
            glPopMatrix();
        }
    
    glPopMatrix();
    glutSwapBuffers();
}

void update(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void onMouseMove(int x, int y) {
     mouseX = x;
     mouseY = y;
}

void updateAngle() {
    // Defina a posição do cubo em 3D
    GLfloat cuboX = tankPosX, cuboY = 0.75f, cuboZ = tankPosZ; // Posição do cubo no espaço 3D

    // Matrizes necessárias para a projeção
    GLdouble modelview[16], projection[16];
    GLint viewport[4];
    GLdouble winX, winY, winZ;

    // Obtenha as matrizes e o viewport
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Projeta a posição 3D do cubo nas coordenadas da tela (2D)
    gluProject(cuboX, cuboY, cuboZ, modelview, projection, viewport, &winX, &winY, &winZ);

    // Calcular as coordenadas do centro do cubo em 2D
    int centerX = static_cast<int>(winX);
    int centerY = static_cast<int>(viewport[3] - winY); // Inverte Y

    // Converter a posição do mouse para coordenadas relativas ao centro do cubo
    int dx = mouseX - centerX;
    int dy = centerY - mouseY; // Inverter y porque a origem (0,0) está no topo da tela

    // Calcular o ângulo em relação ao eixo X (no plano XY)
    angleY = atan2((float)dy, (float)dx) * (180.0 / M_PI); // Converter para graus
    angleY = angleY - angleYY;

    // Exibir o valor de angleY no terminal usando printf
    printf("angleY: %.2f degrees\n", angleY);

    // Normalizar o ângulo para ficar entre 0 e 360 graus
    if (angleY < 0) {
        angleY += 360.0f;
    }

    glutPostRedisplay(); // Solicitar atualização da tela
}

void keyboardFunc(unsigned char key, int x, int y) {
     switch (key) {
        case 'w': // Mover para frente na direção que o tanque está apontando
            tankPosZ += speed * sin(angleYY * M_PI / 180.0f);
            tankPosX -= speed * cos(angleYY * M_PI / 180.0f);
            break;
        case 's': // Mover para trás na direção oposta à que o tanque está apontando
           
            break;
        case 'a': // Rotacionar o tanque para a esquerda
            angleYY += rotationSpeed;
            break;
        case 'd': // Rotacionar o tanque para a direita
            angleYY -= rotationSpeed;
            break;
    }
}


void initialize() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h) {
    // Evitar divisão por zero ao calcular a proporção
    if (h == 0) h = 1;

    // Ajustar o viewport para cobrir toda a nova janela
    glViewport(0, 0, w, h);

    // Configurar a projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Definir a projeção perspectiva com base na proporção da janela
    GLfloat aspect = (GLfloat)w / (GLfloat)h;
    gluPerspective(45.0, aspect, 0.1, 100.0);

    // Retornar à matriz de modelagem
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void idleFunc() {
    updateAngle(); // Atualiza o ângulo a cada frame
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tanque 3D com Rodas Laterais");
    initialize();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(onMouseMove); // Captura o movimento do mouse
    glutIdleFunc(idleFunc);
    glutTimerFunc(16, update, 0);
    glutKeyboardFunc(keyboardFunc); // Captura eventos do teclado
    glutMainLoop();
    return 0;
}
