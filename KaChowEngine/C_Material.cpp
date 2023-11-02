#include "C_Material.h"
#include "ModuleGeometry.h"
#include "Application.h"
#include "C_Mesh.h"

C_Material::C_Material() : Component(nullptr)
{
	type = ComponentType::MATERIAL;
	currentTexture = 0;
	mParent = nullptr;
}

C_Material::C_Material(GameObject* parent) : Component(parent)
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
	const char* listTextures[]{ "Texture", "None", "Checker" };

	//Texture component inspector
	if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth))
	{
		std::string pathaux = "Path: " + std::string(texture_path);
		ImGui::TextWrapped(pathaux.c_str());

		int width = mParent->GetMeshComponent()->mesh->texture_width;
		int height = mParent->GetMeshComponent()->mesh->texture_height;

		std::string aux = "Size: " + std::to_string(width) + " x " + std::to_string(height);
		ImGui::Text(aux.c_str());

		ImGui::Text("Texture: ");
		ImGui::SameLine();
		ImGui::Combo("##Choose Texture", &currentTexture, listTextures, IM_ARRAYSIZE(listTextures));

	}

	UpdateMeshTexture();
}

void C_Material::SetTexture(const char* file_path)
{
	if (textureID != 0) {
		App->texture2D->FreeTexture(textureID);
		textureID = 0;
	}
	textureID = App->texture2D->LoadTexture(file_path);
	UpdateMeshTexture();
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
	if (mParent == nullptr) return;

	C_Mesh* cm = mParent->GetMeshComponent();
	if (cm == nullptr || cm->mesh == nullptr) return;

	//Send selected texture
	if (currentTexture == 0) {
		cm->mesh->id_texture = textureID;
		return;
	}

	if (currentTexture == 2) {
		cm->mesh->id_texture = App->texture2D->checkerID;
		return;
	}

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