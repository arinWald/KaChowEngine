#include "C_Mesh.h"

C_Mesh::C_Mesh() : Component(nullptr)
{

}

C_Mesh::C_Mesh(GameObject* parent) : Component(nullptr)
{
}

C_Mesh::~C_Mesh()
{
}

void C_Mesh::OnEditor()
{

}

void C_Mesh::Enable()
{
	active = false;
}

void C_Mesh::Disable()
{
	active = false;
}