#pragma once
#include "Module.h"
#include "Globals.h"
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
	update_status Update(float dt);
	bool CleanUp();

	void PrintHierarchy(GameObject* GO, int index);

	void SetGameObjectSelected(GameObject* gameObject);

	GameObject* CreateGameObject(GameObject* parent);

	GameObject* rootGameObject;
	GameObject* selectedGameObj;
	GameObject* hoveredGameObj;
	GameObject* draggedGameObject;

	GameObject* bakerHouse;

	GameObject* skybox;

	GameObject* currentGameCamera;

	GameObject* prova1;
	GameObject* prova2;
	GameObject* prova3;
	GameObject* prova4;
	GameObject* prova5;
	GameObject* prova6;
};