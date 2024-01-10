#pragma once
#include "Module.h"
#include <vector>
#include <string>
#include "Component.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "Application.h"
#include "OurPrimitive.h"
#include "C_Camera.h"
#include "C_AudioListener.h"

enum class ComponentType;
class Component;
class C_Transform;
class C_Mesh;
class C_Material;
class C_Camera;
class C_AudioListener;
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
	bool AddThisChild(GameObject* gameObject);
	bool IsChildOf(GameObject* parent);
	void DeleteChild(GameObject* child);
	void PrintOnInspector();
	void SetDeletion(bool deleteGO) { deleteGameObject = deleteGO; };


	C_Mesh* GetMeshComponent();
	C_Material* GetMaterialComponent();
	C_Camera* GetCameraComponent();


	std::string name;

	GameObject* mParent;
	std::vector<GameObject*> mChildren;
	std::vector<Component*> mComponents;

	C_Transform* mTransform;

	bool deleteGameObject;
	int componentNum;

	std::string GetUUIDName();
	std::string uuid;

	ShapeType type;

};