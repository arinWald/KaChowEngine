#include "Application.h"
#include "GameObject.h"
#include "ModuleScene.h"


GameObject::GameObject(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

bool GameObject::Update()
{
	return true;
}

void AddComponent(int type)
{
	//Mirar si exite
	// Switch con el type
	C_Tranform* Temp = new C_Transform();
	mTransform = Temp;
	mComponents.push_back(mTransform);
}