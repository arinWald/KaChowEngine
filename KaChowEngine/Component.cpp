#include "Component.h"

Component::Component(GameObject* parent)
{
	type = ComponentType::NONE;
	active = true;
	this->mParent = parent;
}

Component::~Component()
{

}

void Component::OnEditor()
{

}

void Component::Update()
{

}

void Component::Enable()
{
	active = false;
}

void Component::Disable()
{
	active = false;
}