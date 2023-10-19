#pragma once


enum class ComponentType
{
	NONE,
	TRANSFORM,
	MESH,
	MATERIAL,
};

class GameObject;
class Component
{
public:
	Component(GameObject* parent);
	virtual ~Component();

	virtual void OnEditor();
	virtual void Update();

	void Enable();
	void Disable();

	bool active;
	ComponentType type;
	GameObject* mParent;
};


