// =============================================================
//  lighting.cpp  —  PERSON 3 (Lighting)
//
//  Owns:
//    - initLighting()   : Lab 7  GL_LIGHT0 white + GL_LIGHT1 warm
//    - applyLighting()  : Lab 7  per-frame toggle + position update
//    - glMaterialfv     : Lab 7  specular + shininess
//    - GL_CULL_FACE     : Labs 2-7  backface culling toggle
//    - drawSun()        : Lab 5  unlit sphere at light position
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

#include "lighting.h"
#include "scene.h"        // needs useCulling
#include "helicopter.h"   // needs drawCube for sun sphere

// ---- light toggle state ----
bool light0On = true;
bool light1On = true;

// =============================================================
//  Lab 7 : lighting setup
// =============================================================
void initLighting()
{
    glEnable(GL_LIGHTING);

    // white sun light (GL_LIGHT0)
    GLfloat whiteAmb[]  = {0.25f,0.25f,0.25f,1};
    GLfloat whiteDiff[] = {1.0f, 1.0f, 1.0f, 1};
    GLfloat whiteSpec[] = {1.0f, 1.0f, 1.0f, 1};
    GLfloat whitePos[]  = {6.0f, 9.0f, 6.0f, 1};
    glLightfv(GL_LIGHT0,GL_AMBIENT,  whiteAmb);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,  whiteDiff);
    glLightfv(GL_LIGHT0,GL_SPECULAR, whiteSpec);
    glLightfv(GL_LIGHT0,GL_POSITION, whitePos);

    // warm fill light (GL_LIGHT1)
    GLfloat warmAmb[]  = {0.1f,0.05f,0.0f,1};
    GLfloat warmDiff[] = {1.0f,0.7f, 0.3f,1};
    GLfloat warmPos[]  = {-6.0f,5.0f,-4.0f,1};
    glLightfv(GL_LIGHT1,GL_AMBIENT,  warmAmb);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,  warmDiff);
    glLightfv(GL_LIGHT1,GL_POSITION, warmPos);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    // Lab 7: per-surface material properties
    GLfloat matSpec[] = {0.6f,0.6f,0.6f,1};
    GLfloat shine[]   = {40.0f};
    glMaterialfv(GL_FRONT,GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT,GL_SHININESS,shine);

    // glColor3f drives ambient + diffuse automatically
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
}

// =============================================================
//  Called every frame to apply toggles and re-send positions
// =============================================================
void applyLighting()
{
    // Lab 7: light on/off toggles
    if(light0On) glEnable(GL_LIGHT0); else glDisable(GL_LIGHT0);
    if(light1On) glEnable(GL_LIGHT1); else glDisable(GL_LIGHT1);

    // Labs 2-7: backface culling toggle
    if(useCulling){ glEnable(GL_CULL_FACE); glCullFace(GL_BACK); }
    else           glDisable(GL_CULL_FACE);

    // Re-send world-space positions after modelview is set
    GLfloat wp[]={6,9,6,1}, yp[]={-6,5,-4,1};
    glLightfv(GL_LIGHT0,GL_POSITION,wp);
    glLightfv(GL_LIGHT1,GL_POSITION,yp);
}

// =============================================================
//  Lab 5 : unlit sphere showing where the sun light is
// =============================================================
void drawSun()
{
    glDisable(GL_LIGHTING);
    glColor3f(1.0f,0.95f,0.6f);
    glPushMatrix();
    glTranslatef(6,9,6);
    glutSolidSphere(0.5,20,20);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}
