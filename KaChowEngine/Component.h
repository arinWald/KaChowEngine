#pragma once

#include <string>
enum class ComponentType
{
	NONE,
	TRANSFORM,
	MESH,
	MATERIAL,
	CAMERA,
};

class GameObject;
class Component
{
public:
	Component(GameObject* parent, std::string uuid);
	virtual ~Component();

	virtual void OnEditor();
	virtual void Update();

	void Enable();
	void Disable();

	bool active;
	ComponentType type;
	GameObject* mParent;

	std::string uuid;
};


