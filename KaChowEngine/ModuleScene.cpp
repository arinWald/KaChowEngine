#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"


ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

bool ModuleScene::Init()
{

	return true;
}

bool ModuleScene::Start()
{

	return true;
}

bool ModuleScene::CleanUp()
{
	return true;
}

GameObject* CreateGameObject()
{
	return nullptr;
}
