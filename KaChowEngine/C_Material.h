#pragma once
#include "Globals.h"
#include "GameObject.h"
#include "Component.h"
#include "Glew/include/glew.h"

class GameObject;
class Component;

class C_Material : public Component
{
public:
	C_Material();
	C_Material(GameObject* parent);
	~C_Material();

	GLuint textureID;

	void OnEditor() override;

	virtual void Enable();
	virtual void Disable();
	/*virtual Update();*/


};