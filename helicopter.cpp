// =============================================================
//  helicopter.cpp  —  PERSON 1 (Helicopter)
//
//  Owns:
//    - drawCube()       : Lab 2  manual GL_QUADS cube
//    - box()            : Lab 3  coloured scaled box helper
//    - helicopter()     : Lab 3,4,5  full heli model + rotors
//    - crash system     : EXTRA  collision, debris, fire physics
//    - resetScene()     : EXTRA  restart after crash
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

#include <cstdlib>
#include <cmath>
#include "helicopter.h"

// ---- definitions of all extern variables ----
float heliX = -5.5f, heliY = 2.2f, heliZ = 0.0f;
float rotorAngle = 0.0f;
float tailAngle  = 0.0f;
bool  crashed    = false;

std::vector<Piece> debris;
std::vector<Piece> fire;

// ---- buildings (needed for crash check) ----
// These mirror the values in scene.cpp exactly.
struct _Bld { float x,z,w,d,h; };
static _Bld _blds[] = {
    {0.0f, 0.0f, 1.6f, 1.6f, 3.5f},
    {-3.5f,-2.5f,1.2f, 1.2f, 2.0f},
    {3.0f, 2.2f, 1.3f, 1.3f, 2.6f},
    {-3.0f,2.5f, 1.1f, 1.1f, 1.8f},
    {3.2f,-2.5f,1.2f, 1.2f, 2.4f}
};

// =============================================================
//  Lab 2 : manual cube with normals + texture coords
// =============================================================
void drawCube()
{
    glBegin(GL_QUADS);
    // front
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(-0.5f,-0.5f, 0.5f);
    glTexCoord2f(1,0); glVertex3f( 0.5f,-0.5f, 0.5f);
    glTexCoord2f(1,1); glVertex3f( 0.5f, 0.5f, 0.5f);
    glTexCoord2f(0,1); glVertex3f(-0.5f, 0.5f, 0.5f);
    // back
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f( 0.5f,-0.5f,-0.5f);
    glTexCoord2f(1,0); glVertex3f(-0.5f,-0.5f,-0.5f);
    glTexCoord2f(1,1); glVertex3f(-0.5f, 0.5f,-0.5f);
    glTexCoord2f(0,1); glVertex3f( 0.5f, 0.5f,-0.5f);
    // left
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0); glVertex3f(-0.5f,-0.5f,-0.5f);
    glTexCoord2f(1,0); glVertex3f(-0.5f,-0.5f, 0.5f);
    glTexCoord2f(1,1); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0,1); glVertex3f(-0.5f, 0.5f,-0.5f);
    // right
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); glVertex3f( 0.5f,-0.5f, 0.5f);
    glTexCoord2f(1,0); glVertex3f( 0.5f,-0.5f,-0.5f);
    glTexCoord2f(1,1); glVertex3f( 0.5f, 0.5f,-0.5f);
    glTexCoord2f(0,1); glVertex3f( 0.5f, 0.5f, 0.5f);
    // top
    glNormal3f(0,1,0);
    glTexCoord2f(0,0); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(1,0); glVertex3f( 0.5f, 0.5f, 0.5f);
    glTexCoord2f(1,1); glVertex3f( 0.5f, 0.5f,-0.5f);
    glTexCoord2f(0,1); glVertex3f(-0.5f, 0.5f,-0.5f);
    // bottom
    glNormal3f(0,-1,0);
    glTexCoord2f(0,0); glVertex3f(-0.5f,-0.5f,-0.5f);
    glTexCoord2f(1,0); glVertex3f( 0.5f,-0.5f,-0.5f);
    glTexCoord2f(1,1); glVertex3f( 0.5f,-0.5f, 0.5f);
    glTexCoord2f(0,1); glVertex3f(-0.5f,-0.5f, 0.5f);
    glEnd();
}

// Lab 3: coloured scaled box helper
void box(float sx, float sy, float sz, float r, float g, float b)
{
    glColor3f(r, g, b);
    glPushMatrix();
    glScalef(sx, sy, sz);
    drawCube();
    glPopMatrix();
}

// =============================================================
//  Lab 4 + 5 : helicopter model with animated rotors
// =============================================================
static void blade(float rot)
{
    glPushMatrix();
    glRotatef(rot, 0,1,0);
    glTranslatef(1.1f, 0, 0);
    box(2.2f, 0.05f, 0.16f, 0.05f,0.05f,0.05f);
    glPopMatrix();
}

void helicopter()
{
    if (crashed) return;
    glPushMatrix();
    glTranslatef(heliX, heliY, heliZ);
    glRotatef(180, 0,1,0);  // nose faces +x (toward buildings)

    // body  — Lab 3 transforms
    box(1.2f, 0.45f, 0.7f, 0.85f,0.05f,0.05f);

    // nose
    glPushMatrix(); glTranslatef(-0.75f,0,0);
    box(0.4f,0.35f,0.5f, 1.0f,0.15f,0.1f); glPopMatrix();

    // Lab 5: glutSolidSphere cockpit dome
    glColor3f(0.4f,0.7f,1.0f);
    glPushMatrix(); glTranslatef(-0.55f,0.18f,0);
    glScalef(1,1,1.1f);
    glutSolidSphere(0.32, 24, 24);
    glPopMatrix();

    // tail boom
    glPushMatrix(); glTranslatef(1.2f,0.05f,0);
    box(1.6f,0.15f,0.15f, 0.9f,0.8f,0.1f); glPopMatrix();

    // tail fin
    glPushMatrix(); glTranslatef(1.95f,0.25f,0);
    box(0.25f,0.5f,0.08f, 0.85f,0.05f,0.05f); glPopMatrix();

    // landing skids
    glPushMatrix(); glTranslatef(0,-0.4f, 0.35f);
    box(1.3f,0.06f,0.06f, 0.2f,0.2f,0.2f); glPopMatrix();
    glPushMatrix(); glTranslatef(0,-0.4f,-0.35f);
    box(1.3f,0.06f,0.06f, 0.2f,0.2f,0.2f); glPopMatrix();

    // Lab 4: animated main rotor
    glPushMatrix(); glTranslatef(0,0.55f,0);
    glRotatef(rotorAngle, 0,1,0);
    blade(0); blade(90); blade(180); blade(270);
    glPopMatrix();

    // animated tail rotor
    glPushMatrix(); glTranslatef(1.95f,0.25f,0.12f);
    glRotatef(tailAngle,0,0,1);
    box(0.5f,0.05f,0.05f, 0.05f,0.05f,0.05f);
    glPushMatrix(); glRotatef(90,0,0,1);
    box(0.5f,0.05f,0.05f, 0.05f,0.05f,0.05f); glPopMatrix();
    glPopMatrix();

    glPopMatrix();
}

// =============================================================
//  EXTRA : crash + particle physics
// =============================================================
void makeCrash(float bx, float bz)
{
    crashed = true;
    debris.clear(); fire.clear();
    // building rubble
    for (int i=0;i<70;i++){
        Piece p;
        p.x = bx+((rand()%100)/100.0f-0.5f)*1.5f;
        p.y = 0.8f+(rand()%300)/100.0f;
        p.z = bz+((rand()%100)/100.0f-0.5f)*1.5f;
        p.vx=((rand()%100)/100.0f-0.5f)*0.14f;
        p.vy=0.06f+(rand()%100)/900.0f;
        p.vz=((rand()%100)/100.0f-0.5f)*0.14f;
        p.s=0.18f; p.r=0.4f; p.g=0.4f; p.b=0.42f;
        p.rot=rand()%360; p.rotSpeed=3+rand()%8;
        debris.push_back(p);
    }
    // helicopter fragments
    for (int i=0;i<22;i++){
        Piece p;
        p.x=heliX; p.y=heliY; p.z=heliZ;
        p.vx=((rand()%100)/100.0f-0.5f)*0.22f;
        p.vy=0.08f+(rand()%100)/700.0f;
        p.vz=((rand()%100)/100.0f-0.5f)*0.22f;
        p.s=0.22f; p.r=0.9f; p.g=0.1f; p.b=0.0f;
        p.rot=rand()%360; p.rotSpeed=6+rand()%12;
        debris.push_back(p);
    }
}

void checkCrash()
{
    if (crashed) return;
    for (int i=0;i<5;i++){
        bool inX = heliX > _blds[i].x-_blds[i].w/2 && heliX < _blds[i].x+_blds[i].w/2;
        bool inZ = heliZ > _blds[i].z-_blds[i].d/2 && heliZ < _blds[i].z+_blds[i].d/2;
        bool low = heliY < _blds[i].h+0.5f;
        if (inX && inZ && low){ makeCrash(_blds[i].x,_blds[i].z); return; }
    }
    if (heliY < 0.3f) makeCrash(heliX, heliZ);
}

void spawnFire()
{
    for (int i=0;i<5;i++){
        Piece f;
        f.x=heliX+((rand()%100)/100.0f-0.5f)*0.9f;
        f.y=0.4f;
        f.z=heliZ+((rand()%100)/100.0f-0.5f)*0.9f;
        f.vx=((rand()%100)/100.0f-0.5f)*0.025f;
        f.vy=0.05f+(rand()%100)/3000.0f;
        f.vz=((rand()%100)/100.0f-0.5f)*0.025f;
        f.s=0.18f;
        if (rand()%2){ f.r=1; f.g=0.25f; f.b=0; }
        else         { f.r=1; f.g=0.85f; f.b=0; }
        f.rot=0; f.rotSpeed=0;
        fire.push_back(f);
    }
    if (fire.size()>120) fire.erase(fire.begin(),fire.begin()+15);
}

void drawDebris()
{
    for (size_t i=0;i<debris.size();i++){
        Piece &p=debris[i];
        glPushMatrix();
        glTranslatef(p.x,p.y,p.z);
        glRotatef(p.rot,1,1,0);
        box(p.s,p.s,p.s,p.r,p.g,p.b);
        glPopMatrix();
    }
}

void drawFire()
{
    glDisable(GL_LIGHTING);
    for (size_t i=0;i<fire.size();i++){
        Piece &f=fire[i];
        glColor3f(f.r,f.g,f.b);
        glPushMatrix();
        glTranslatef(f.x,f.y,f.z);
        glScalef(f.s,f.s,f.s);
        drawCube();
        glPopMatrix();
    }
    glEnable(GL_LIGHTING);
}

void resetScene()
{
    crashed=false; debris.clear(); fire.clear();
    heliX=-5.5f; heliY=2.2f; heliZ=0.0f;
}

void updateParticles()
{
    if (!crashed){
        rotorAngle+=18.0f; if(rotorAngle>360) rotorAngle-=360;
        tailAngle +=30.0f; if(tailAngle >360) tailAngle -=360;
    } else {
        spawnFire();
        for (size_t i=0;i<debris.size();i++){
            Piece &p=debris[i];
            p.x+=p.vx; p.y+=p.vy; p.z+=p.vz;
            p.vy-=0.006f;
            if(p.y<0.1f){ p.y=0.1f; p.vy*=-0.25f; p.vx*=0.92f; p.vz*=0.92f; }
            p.rot+=p.rotSpeed;
        }
        for (size_t i=0;i<fire.size();i++){
            Piece &f=fire[i];
            f.x+=f.vx; f.y+=f.vy; f.z+=f.vz;
            f.s*=0.985f;
        }
    }
}
