#include "GameObject.h"


GameObject::GameObject()
{
	name = "GameObject";
	mParent = nullptr;
	mTransform = new C_Transform();

	mComponents.push_back(mTransform);
}

GameObject::~GameObject()
{
	name = "";
	mParent = nullptr;
	mTransform = nullptr;

	// pilota (delete del reves?)
	for (size_t i = mComponents.size(); i >= 0; --i)
	{
		delete mComponents[i];
		mComponents[i] = nullptr;
	}

	for (size_t i = mChildren.size(); i >= 0; --i)
	{
		delete mChildren[i];
		mChildren[i] = nullptr;
	}

	mComponents.clear();
}

GameObject::GameObject(GameObject* parent)
{
	name = "GameObject";
	this->mParent = parent;

	if (parent != nullptr)
	{
		parent->mChildren.push_back(this);
	}

	mTransform = new C_Transform();

	mComponents.push_back(mTransform);
}

void GameObject::AddComponent(ComponentType type)
{
	Component* newComponent = new Component(this);

	newComponent->type = type;
	newComponent->active = true;

	mComponents.push_back(newComponent);

	delete newComponent;
}


void GameObject::Update()
{
	// No tots els children o components tenen un update
	for (size_t i = 0; i < mChildren.size(); ++i)
	{
		mChildren[i]->Update();
	}

	for (size_t i = 0; i < mComponents.size(); ++i)
	{
		mComponents[i]->Update();
	}
}
