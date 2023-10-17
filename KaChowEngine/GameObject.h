#pragma once
#include "Module.h"
#include <vector>

class GameObject : public Module
{
public:

	GameObject(Application* app, bool start_enabled = true);
	~GameObject() 
	{
		//mTransform = nullptr;
		//for (size_t i = mComponents.size(); i >= 0; --i)
		//{
		//	delete mComponents[i];
		//	mComponents[i] = nullptr;
		//}

		//for (size_t i = mChildren.size(); i >= 0; --i)
		//{
		//	delete mChildren[i];
		//	mChildren[i] = nullptr;
		//}
	}

	//AddComponent(type)
	//{
	//	//Mirar si exite
	//	// Switch con el type
	//	C_Tranform* Temp = new C_Transform();
	//	mTransform = Temp;
	//	mComponents.push_back(mTransform);
	//}

	bool Init();
	bool Start();
	bool CleanUp();
	bool Update();

	std::string name;


	GameObject* mParent;
	std::vector<GameObject*> mChildren;
	//std::vector<Component*> mComponents;



	/*Component_Transform* mTransform;*/
};

class GameObject;
class Component : GameObject
{
	Component();
	~Component();

	enum Type
	{

	};

	bool active;

	GameObject* mParent;
};

//class C_Transform : public Component
//{
//	void OnEditor() override;
//
//	virtual Enable();
//	virtual Update();
//	virtual Disable();
//
//	float3 mPosition;
//	float3 mScale;
//	Quat mRotation;
//
//	float4x4 mGlobalMatrix:
//	float4x4 mLocalMatrix;
//};