#pragma once
#include "Globals.h"
#include "GameObject.h"
#include "Component.h"

class GameObject;
class Component;
struct Mesh;
class C_Mesh : public Component
{
public:
	C_Mesh();
	C_Mesh(GameObject* parent);
	~C_Mesh();

	void OnEditor() override;

	virtual void Enable();
	virtual void Disable();
	/*virtual Update();*/

	Mesh* mesh;

};