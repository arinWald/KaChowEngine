#include "C_Material.h"
#include "ModuleGeometry.h"
#include "Application.h"
#include "C_Mesh.h"

C_Material::C_Material(std::string uuid) : Component(nullptr, uuid)
{
	type = ComponentType::MATERIAL;
	currentTexture = TEXTURE;
	mParent = nullptr;
}

C_Material::C_Material(GameObject* parent, std::string uuid) : Component(parent, uuid)
{
	type = ComponentType::MATERIAL;
	this->mParent = parent;
	currentTexture = TEXTURE;
}

C_Material::~C_Material()
{
}

void C_Material::OnEditor()
{
	//const char* textureList[]{ "Texture", "None", "Checker" };

	////Texture component inspector
	//if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth))
	//{
	//	int width = mParent->GetMeshComponent()->meshes[0]->texture_width;
	//	int height = mParent->GetMeshComponent()->meshes[0]->texture_height;

	//	std::string aux = "Size: " + std::to_string(width) + " x " + std::to_string(height);
	//	ImGui::TextWrapped(aux.c_str());

	//	std::string pathaux = "Path: " + std::string(texture_path);
	//	ImGui::TextWrapped(pathaux.c_str());

	//	ImGui::Text("Change Texture: ");
	//	int txtType = currentTexture;
	//	ImGui::Combo("##Choose Texture", &txtType, textureList, IM_ARRAYSIZE(textureList));
	//	currentTexture = (CurrentTextureType)txtType;

	//}

	//UpdateMeshTexture();
}

void C_Material::SetTexture(const char* file_path)
{
	if (textureID != 0) {
		App->texture2D->FreeTexture(textureID);
		textureID = 0;
		texture_path = file_path;
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
	for (int i = 0; i < mParent->GetMeshComponent()->meshes.size(); i++)
	{
		if (cm == nullptr || cm->meshes[i] == nullptr) return;

		//Send selected texture
		if (currentTexture == TEXTURE) {
			cm->meshes[i]->id_texture = textureID;
			return;
		}
		else if (currentTexture == CHECKERS) {
			cm->meshes[i]->id_texture = App->texture2D->checkerID;
			return;
		}
		cm->meshes[i]->id_texture = 0;
	}
	
}

void C_Material::Enable()
{
	active = false;
}

void C_Material::Disable()
{
	active = false;
}