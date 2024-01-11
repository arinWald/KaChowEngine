#include "Component.h"

int audioIdCounter = 0;

Component::Component(GameObject* parent, std::string uuid)
{
	type = ComponentType::NONE;
	active = true;
	this->mParent = parent;

	this->uuid = uuid;
}

Component::Component(GameObject* parent, std::string uuid, int audioId)
{
	type = ComponentType::NONE;
	active = true;
	this->mParent = parent;

	this->uuid = uuid;
	audioId = audioIdCounter;
	this->audioId = audioId;
	audioIdCounter++;
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