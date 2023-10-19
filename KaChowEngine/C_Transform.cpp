#include "C_Transform.h"

C_Transform::C_Transform() : Component(nullptr)
{
	
}

C_Transform::C_Transform(GameObject* parent) : Component(nullptr)
{
	type = ComponentType::TRANSFORM;
	this->mParent = parent;

	mPosition = { 0, 0, 0 };
	mScale = { 0, 0, 0 };
	mRotation = { 0, 0, 0, 0};
}

C_Transform::~C_Transform()
{
}

void C_Transform::OnEditor()
{

}

void C_Transform::Enable()
{
	active = false;
}

void C_Transform::Disable()
{
	active = false;
}

void C_Transform::SetTransfoMatrix(float3 position, float3 rotation, float3 scale)
{
	// Pilota
}
