#pragma once

#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Module.h"
#include "Globals.h"
#include <vector>

#include "Glew/include/glew.h"


class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init() override;
	void DrawEditor();
	bool CleanUp() override;

	void AddFPS(const float aFPS);

	std::vector<float> mFPSLog;

private:
	int cvCounter;

	// Config window 
	bool fullscreen;
	float screenWidth;

	// Config render opengl
	bool lightning;
	bool isCullFace;
	bool depthTest;
	bool colorMat;
	bool texture2D;
	bool vSync;
	bool isWireframe;
	float bright;
	float bright_aux;
};

#endif