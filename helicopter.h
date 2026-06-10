#ifndef HELICOPTER_H
#define HELICOPTER_H

// Helicopter position and rotor angles (defined in helicopter.cpp)
extern float heliX, heliY, heliZ;
extern float rotorAngle, tailAngle;
extern bool  crashed;

// Particle vectors (defined in helicopter.cpp)
#include <vector>
struct Piece {
    float x, y, z;
    float vx, vy, vz;
    float s;
    float r, g, b;
    float rot, rotSpeed;
};
extern std::vector<Piece> debris;
extern std::vector<Piece> fire;

// Functions
void drawCube();
void box(float sx, float sy, float sz, float r, float g, float b);
void helicopter();
void makeCrash(float bx, float bz);
void checkCrash();
void spawnFire();
void drawDebris();
void drawFire();
void resetScene();
void updateParticles();

#endif
