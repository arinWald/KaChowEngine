#pragma once
#include "Globals.h"
#include "GameObject.h"
#include "Component.h"
#include "ImGui/imgui.h"
#include <string>

class GameObject;
class Component;
struct Mesh;
class C_Mesh : public Component
{
public:
	C_Mesh(std::string uuid);
	C_Mesh(GameObject* parent, std::string uuid);
	~C_Mesh();

	void OnEditor() override;

	virtual void Enable();
	virtual void Disable();
	virtual void Update();

	void UpdateBBData();

	Mesh* mesh;

	bool showNormals;

};