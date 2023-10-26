#pragma once
#include "Module.h"
#include "Globals.h"
#include <vector>

class GameObject;
class ModuleScene : public Module
{
public:

	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene() {}

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void PrintHierarchy(GameObject* GO, int index);

	// Pilota
	GameObject* CreateGameObject(GameObject* parent);

	GameObject* rootGameObject;
	GameObject* selectedGameObj;
	GameObject* hoveredGameObj;
	GameObject* draggedGameObject;
};