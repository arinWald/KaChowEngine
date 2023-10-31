#pragma once
#include "Module.h"
#include <vector>
#include <string>
#include "Component.h"
#include "C_Transform.h"
#include "C_Mesh.h"

enum class ComponentType;
class Component;
class C_Transform;
class C_Mesh;
class GameObject
{
public:

	GameObject();
	GameObject(GameObject* parent);
	~GameObject();

	void AddComponent(Component* component);
	void Update();
	GameObject* GetParent();
	bool SetNewParent(GameObject* newParent);
	bool SetAsChildOf(GameObject* gameObject);
	bool IsChildOf(GameObject* parent);
	void DeleteChild(GameObject* child);


	C_Mesh* GetMeshComponent();


	std::string name;

	GameObject* mParent;
	std::vector<GameObject*> mChildren;
	std::vector<Component*> mComponents;

	C_Transform* mTransform;

};