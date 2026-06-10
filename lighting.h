#ifndef LIGHTING_H
#define LIGHTING_H

// Light toggle state (defined in lighting.cpp)
extern bool light0On;
extern bool light1On;

// Functions
void initLighting();
void applyLighting();
void drawSun();

#endif
