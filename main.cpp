// =============================================================
//  main.cpp  —  SHARED (everyone's files plug in here)
//
//  Owns:
//    - display()    : calls all three parts
//    - reshape()    : Lab 2-8  glFrustum projection
//    - update()     : Lab 4    idle animation loop
//    - keyboard()   : EXTRA    all controls
//    - main()       : initialises everything and starts GLUT
//
//  Controls:
//    w/s       move heli forward / back
//    a/d       move heli left / right
//    q/e       move heli up / down
//    j/l       rotate camera left / right
//    i/k       raise / lower camera
//    +/-       zoom in / out
//    1/0       white light  ON / OFF
//    7/8       warm light   ON / OFF
//    t         toggle textures
//    c         toggle backface culling
//    r         reset after crash
//    Esc       quit
// =============================================================

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "helicopter.h"
#include "scene.h"
#include "lighting.h"

// =============================================================
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    setupCamera();      // scene.cpp  — Lab 6 gluLookAt
    applyLighting();    // lighting.cpp — Lab 7 lights

    drawSun();          // lighting.cpp — Lab 5 sphere
    drawGround();       // scene.cpp   — Lab 8 texture
    drawBuildings();    // scene.cpp   — Lab 3 transforms
    helicopter();       // helicopter.cpp — Lab 3,4,5

    if(crashed){
        drawDebris();   // helicopter.cpp — EXTRA
        drawFire();     // helicopter.cpp — EXTRA
    }

    glutSwapBuffers();
}

// Lab 2-8: teacher used glFrustum for perspective
void reshape(int w, int h)
{
    if(h==0) h=1;
    float ar=(float)w/h;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Lab 4: idle loop for animation + EXTRA physics
void update()
{
    updateParticles();   // helicopter.cpp
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    if(!crashed){
        if(key=='w') heliX+=0.25f;
        else if(key=='s') heliX-=0.25f;
        else if(key=='a') heliZ-=0.25f;
        else if(key=='d') heliZ+=0.25f;
        else if(key=='q') heliY+=0.25f;
        else if(key=='e') heliY-=0.25f;
        checkCrash();
    }
    if(key=='j') camAngle-=5.0f;
    else if(key=='l') camAngle+=5.0f;
    else if(key=='i') camHeight+=0.5f;
    else if(key=='k') camHeight-=0.5f;
    else if(key=='+'||key=='=') camRadius-=0.6f;
    else if(key=='-') camRadius+=0.6f;
    else if(key=='1') light0On=true;
    else if(key=='0') light0On=false;
    else if(key=='7') light1On=true;
    else if(key=='8') light1On=false;
    else if(key=='t') useTexture=!useTexture;
    else if(key=='c') useCulling=!useCulling;
    else if(key=='r') resetScene();
    else if(key==27)  exit(0);

    glutPostRedisplay();
}

// =============================================================
int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900,650);
    glutCreateWindow("Helicopter Crash Scene - CG Project");

    glClearColor(0.55f,0.8f,1.0f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    initLighting();   // lighting.cpp — Lab 7
    initTextures();   // scene.cpp    — Lab 8

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(update);

    glutMainLoop();
    return 0;
}
