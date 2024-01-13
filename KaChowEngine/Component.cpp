#include "Component.h"
#include "Globals.h"

int audioIdCounter = 0;

Component::Component(GameObject* parent, std::string uuid)
{
	type = ComponentType::NONE;
	active = true;
	this->mParent = parent;

	this->uuid = uuid;


	audioId = audioIdCounter;
	audioIdCounter++;
	LOG("Component ID:  %d", audioIdCounter);
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