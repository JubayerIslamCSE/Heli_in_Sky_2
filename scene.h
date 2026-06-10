#ifndef SCENE_H
#define SCENE_H

// Camera state (defined in scene.cpp)
extern float camAngle;
extern float camHeight;
extern float camRadius;

// Texture toggles (defined in scene.cpp)
extern bool useTexture;
extern bool useCulling;

// Functions
void initTextures();
void drawGround();
void drawBuildings();
void setupCamera();

#endif
// done
