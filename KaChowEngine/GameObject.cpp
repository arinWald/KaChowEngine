#include "Application.h"
#include "GameObject.h"
#include "ModuleScene.h"


GameObject::GameObject(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

bool GameObject::Init()
{

	return true;
}

bool GameObject::Start()
{

	return true;
}

bool GameObject::Update()
{
	return true;
}


bool GameObject::CleanUp()
{
	return true;
}