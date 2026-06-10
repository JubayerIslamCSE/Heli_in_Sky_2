// =============================================================
//  scene.cpp  —  PERSON 2 (Scene / Environment)
//
//  Owns:
//    - initTextures()   : Lab 8  procedural brick + grass textures
//    - drawGround()     : Lab 8  textured ground quad
//    - drawBuildings()  : Lab 3  5 buildings using transforms
//    - setupCamera()    : Lab 6  gluLookAt orbiting camera
//    - camAngle/Height  : EXTRA  interactive camera controls
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

#include "scene.h"
#include "helicopter.h"   // needs box() and drawCube()

// ---- camera state ----
float camAngle  = 45.0f;
float camHeight = 6.0f;
float camRadius = 13.0f;

// ---- toggles ----
bool useTexture = true;
bool useCulling = true;

// ---- texture ids ----
static unsigned int groundTex = 0;
static unsigned int brickTex  = 0;

// =============================================================
//  Lab 8 : procedural textures (no image files needed)
// =============================================================
static unsigned int makeChecker(int r1,int g1,int b1,
                                int r2,int g2,int b2)
{
    const int N = 64;
    unsigned char data[64][64][3];
    for (int i=0;i<N;i++)
        for (int j=0;j<N;j++){
            bool c = ((i/8)+(j/8))&1;
            data[i][j][0]=c?r1:r2;
            data[i][j][1]=c?g1:g2;
            data[i][j][2]=c?b1:b2;
        }
    unsigned int id;
    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_2D,id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,N,N,GL_RGB,GL_UNSIGNED_BYTE,data);
    return id;
}

static unsigned int makeBrick()
{
    const int N = 64;
    unsigned char data[64][64][3];
    for (int i=0;i<N;i++)
        for (int j=0;j<N;j++){
            int row=i/16;
            int off=(row%2)?16:0;
            int jj=(j+off)%64;
            bool mortar=(i%16<2)||(jj%32<2);
            if(mortar){data[i][j][0]=210;data[i][j][1]=210;data[i][j][2]=205;}
            else      {data[i][j][0]=150;data[i][j][1]=55; data[i][j][2]=40; }
        }
    unsigned int id;
    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_2D,id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,N,N,GL_RGB,GL_UNSIGNED_BYTE,data);
    return id;
}

void initTextures()
{
    groundTex = makeChecker(60,140,60, 40,110,45);
    brickTex  = makeBrick();
}

// =============================================================
//  Lab 8 : textured ground
// =============================================================
void drawGround()
{
    glColor3f(1,1,1);
    if(useTexture){ glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D,groundTex); }
    glNormal3f(0,1,0);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(-8,0,-7);
        glTexCoord2f(8,0); glVertex3f( 8,0,-7);
        glTexCoord2f(8,8); glVertex3f( 8,0, 7);
        glTexCoord2f(0,8); glVertex3f(-8,0, 7);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// =============================================================
//  Lab 3 : buildings using transforms
// =============================================================
struct Building { float x,z,w,d,h; };
static Building buildings[] = {
    {0.0f, 0.0f, 1.6f, 1.6f, 3.5f},
    {-3.5f,-2.5f,1.2f, 1.2f, 2.0f},
    {3.0f, 2.2f, 1.3f, 1.3f, 2.6f},
    {-3.0f,2.5f, 1.1f, 1.1f, 1.8f},
    {3.2f,-2.5f,1.2f, 1.2f, 2.4f}
};

static void drawOneBuilding(Building b)
{
    if(useTexture){ glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D,brickTex); }
    glColor3f(1,1,1);
    glPushMatrix();
    glTranslatef(b.x, b.h/2.0f, b.z);
    glScalef(b.w, b.h, b.d);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    // roof slab
    glPushMatrix();
    glTranslatef(b.x, b.h+0.03f, b.z);
    box(b.w*0.9f, 0.06f, b.d*0.9f, 0.75f,0.78f,0.82f);
    glPopMatrix();
}

void drawBuildings()
{
    for (int i=0;i<5;i++) drawOneBuilding(buildings[i]);
}

// =============================================================
//  Lab 6 : camera setup with gluLookAt
// =============================================================
void setupCamera()
{
    float camX = camRadius * cosf(camAngle*3.1416f/180.0f);
    float camZ = camRadius * sinf(camAngle*3.1416f/180.0f);
    gluLookAt(camX, camHeight, camZ,  0,1.5f,0,  0,1,0);
}
