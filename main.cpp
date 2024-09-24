#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <cmath>
#include <cstdio>
#include <iostream>

// COMPILAR
#include "components/Tank.cpp"

float angleY = 0.0f; 
float angleYY = -90.0f; 
int mouseX, mouseY;  
float tankPosX = 0.0f, tankPosZ = 0.0f; 
float speed = 8.0f; 
float rotationSpeed = 150.0f; 
bool keyWPressed = false;
bool rotatingA = false;
bool rotatingD = false;
bool cameraRotate = false;


int previousTime = 0;
float worldposX = 0.0f;
float worldposZ = 0.0f;
float lookAtX = 0.0f;
float lookAtZ = 0.0f;
float anguloCamera = 0.0f;

float distance = 0.0f;

void display() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Configuração da projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)800 / (GLfloat)600, 0.1, 100.0);
    glTranslatef(0, -5, -10);
    glRotatef(45, 1, 0, 0.0f);

    // Configuração da câmera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(worldposX, 0, worldposZ + 10,  // Posição da câmera: (0, 0, 10), 10 unidades à frente ao longo do eixo Z
              worldposX, 0.0, worldposZ,  // A câmera está olhando para o ponto (0, 0, 0), o centro da cena
              0.0, 1.0, 0.0); // O vetor "up" está ao longo do eixo Y positivo

    // Desenhar o tanque
        drawTank(tankPosX, tankPosZ, angleYY, angleY);

    glPushMatrix();
        glColor3f(0.8f, 0.0f, 1.0f);  
        glutSolidCube(1.0);
    glPopMatrix();
    
    glutSwapBuffers();
}

void setupLighting() {
    // Ativar iluminação
    glEnable(GL_LIGHTING);

    glEnable(GL_COLOR_MATERIAL);     // Habilitar cor de material
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);
    
    glEnable(GL_LIGHT0); // Usar a luz 0
  

    // Definir posição da luz
    GLfloat lightPos[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Definir propriedades da luz
    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // Definir material do tanque
    GLfloat matDiffuse[] = { 0.0f, 0.8f, 0.0f, 1.0f };  // Cor do tanque
    GLfloat matSpecular[] = { 0.3f, 0.3f, 0.3f, 1.0f };  // Componente especular
    GLfloat matShininess[] = { 20.0f };                   // Brilho do material
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
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

    GLfloat cuboX = tankPosX, cuboY = 0.75f, cuboZ = tankPosZ;

    GLdouble modelview[16], projection[16];
    GLint viewport[4];
    GLdouble winX, winY, winZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Projeta a posição 3D do cubo nas coordenadas da tela (2D)
    gluProject(cuboX, cuboY, cuboZ, modelview, projection, viewport, &winX, &winY, &winZ);

    int centerX = static_cast<int>(winX);
    int centerY = static_cast<int>(viewport[3] - winY); 

    int dx = mouseX - centerX;
    int dy = centerY - mouseY; 

    angleY = atan2((float)dy, (float)dx) * (180.0 / M_PI); 
    angleY = angleY - angleYY;

    if (angleY <= 0) {
        angleY += 360.0f;
    } 

    glutPostRedisplay(); 
}

void keyboardFunc(unsigned char key, int x, int y) {

     switch (key) {
        case 'a': 
            rotatingA = true;
            break;
        case 'd': 
            rotatingD = true;
            break;
        case 'w': 
            keyWPressed = true;
            break;
        case 'p': 
            cameraRotate = true;
            break;
    }

}

void keyboardUpFunc(unsigned char key, int x, int y) {
    if (key == 'w') {
        keyWPressed = false;
    }
    if (key == 'a') {
        rotatingA = false;
    }
     if (key == 'd') {
        rotatingD = false;
    }
    if (key == 'p') {
        cameraRotate = false;
    }
}


void updateTankPosition() {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (currentTime - previousTime) / 1000.0f; // Converte para segundos
    previousTime = currentTime;

    // Movimentar o tanque com base no deltaTime
    if (keyWPressed) {
        tankPosZ += speed * deltaTime * sin(angleYY * M_PI / 180.0f);
        tankPosX -= speed * deltaTime * cos(angleYY * M_PI / 180.0f);
        worldposZ += speed * deltaTime * sin(angleYY * M_PI / 180.0f);
        worldposX -= speed * deltaTime * cos(angleYY * M_PI / 180.0f);
    }

    // Rotacionar o tanque com base no deltaTime
    if (rotatingA) {
        angleYY += rotationSpeed * deltaTime;
    }

    if (rotatingD) {
        angleYY -= rotationSpeed * deltaTime;
    }

    glutPostRedisplay(); 
}


void initialize() {
    glClearColor(1.0, 1.0, 1.0, 1.0);  

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
    updateAngle(); 
    updateTankPosition();
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
    glutKeyboardFunc(keyboardFunc); 
    glutKeyboardUpFunc(keyboardUpFunc);// Captura eventos do teclado
    setupLighting(); 
    glutMainLoop();
    return 0;
}
