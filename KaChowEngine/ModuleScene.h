#pragma once
#include "Module.h"
#include "GameObject.h"
#include <vector>

class GameObject;
class ModuleScene : public Module
{
public:

	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene() {}


	bool Init();
	bool Start();
	bool CleanUp();

	GameObject* CreateGameObject();

	


};