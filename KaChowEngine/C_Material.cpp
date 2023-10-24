#include "C_Material.h"
#include "ModuleGeometry.h"
#include "Application.h"
#include "C_Mesh.h"

C_Material::C_Material() : Component(nullptr)
{
	type = ComponentType::MATERIAL;
	mParent = nullptr;
}

C_Material::C_Material(GameObject* parent) : Component(nullptr)
{
	type = ComponentType::MATERIAL;
	this->mParent = parent;
	currentTexture = 0;
}

C_Material::~C_Material()
{
}

void C_Material::OnEditor()
{

}

void C_Material::SetTexture(const char* file_path)
{
	if (textureID != 0) {
		App->texture2D->FreeTexture(textureID);
		textureID = 0;
	}
	textureID = App->texture2D->LoadTexture(file_path);
	//UpdateMeshTexture();
}

void C_Material::ResetTexture()
{
	if (textureID != 0) {
		App->texture2D->FreeTexture(textureID);
		textureID = 0;
	}
	UpdateMeshTexture();
}

void C_Material::UpdateMeshTexture()
{
	C_Mesh* cm = mParent->GetMeshComponent();
	if (cm == nullptr || cm->mesh == nullptr) return;

	//Send selected texture
	if (currentTexture == 0) {
		cm->mesh->id_texture = textureID;
		return;
	}

	//if (currentTexture == 2) {
	//	cm->mesh->id_texture = App->texture2D->checkerID;
	//	return;
	//}

	cm->mesh->id_texture = 0;
}

void C_Material::Enable()
{
	active = false;
}

void C_Material::Disable()
{
	active = false;
}