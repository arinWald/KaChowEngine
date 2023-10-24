#include "C_Mesh.h"
#include "Application.h"

C_Mesh::C_Mesh() : Component(nullptr)
{
	type = ComponentType::MESH;
	mesh = nullptr;
}

C_Mesh::C_Mesh(GameObject* parent) : Component(nullptr)
{
	
}

C_Mesh::~C_Mesh()
{
	App->geoLoader->DestroyMesh(mesh);
	mesh = nullptr;
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