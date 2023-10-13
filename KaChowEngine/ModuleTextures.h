#pragma once
#include "Module.h"
#include "Glew/include/glew.h"
#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"
#include "SDL/include/SDL_opengl.h"

#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

class ModuleTextures : public Module
{
public:

	ModuleTextures(Application* app, bool start_enabled = true);
	~ModuleTextures() {}

private:
	bool Init();
	bool Start();
	bool CleanUp();

public:

private:
	GLuint textureID;

	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4]; // height width rgba
};