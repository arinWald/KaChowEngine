#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleGeometry.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"

#include<vector>

class Modules;
class ModuleWindow;
class ModuleInput;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleEditor;
class ModuleGeometry;
class ModuleTextures;
class ModuleScene;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleGeometry* geoLoader;
	ModuleRenderer3D* renderer3D;
	ModuleTextures* texture2D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	ModuleScene* scene;

private:

	Timer	ms_timer;
	float	dt;
	std::vector<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	float GetDT();
	void LOGToEditor(const char* tmp_string);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;