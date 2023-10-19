#include "C_Material.h"
#include "ModuleGeometry.h"

C_Material::C_Material() : Component(nullptr)
{
	type = ComponentType::MATERIAL;
	mParent = nullptr;
}

C_Material::C_Material(GameObject* parent) : Component(nullptr)
{
	type = ComponentType::MATERIAL;
	this->mParent = parent;
}

C_Material::~C_Material()
{
}

void C_Material::OnEditor()
{

}

void C_Material::Enable()
{
	active = false;
}

void C_Material::Disable()
{
	active = false;
}