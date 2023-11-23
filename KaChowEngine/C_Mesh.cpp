#include "C_Mesh.h"
#include "Application.h"


C_Mesh::C_Mesh() : Component(nullptr)
{
	type = ComponentType::MESH;
	mesh = nullptr;
    showNormals = false;
}

C_Mesh::C_Mesh(GameObject* parent) : Component(parent)
{
    type = ComponentType::MESH;
    mesh = nullptr;
    showNormals = false;
}

C_Mesh::~C_Mesh()
{
	App->geoLoader->DestroyMesh(mesh);
	mesh = nullptr;
}

void C_Mesh::OnEditor()
{
    if (ImGui::CollapsingHeader("Mesh"))
    {
        if (mesh == nullptr) return;
        ImGui::LabelText("##%f", "Number of vertex:");
        ImGui::SameLine();
        ImGui::Text("%d", mesh->num_vertex);
        ImGui::LabelText("##%f", "Number of index:");

        // Not printing the proper number (should be a third of vertex?)
        //ImGui::SameLine();
        //ImGui::Text("%d", mesh->num_index);
        ImGui::Checkbox("Face Normals", &showNormals);
    }
}

void C_Mesh::Enable()
{
	active = false;
}

void C_Mesh::Disable()
{
	active = false;
}

void C_Mesh::UpdateBBData()
{
    mesh->OBB_box = mesh->AABB_box;
    mesh->OBB_box.Transform(mesh->owner->mTransform->getGlobalMatrix().Transposed());
    mesh->Global_AABB_box.SetNegativeInfinity();
    mesh->Global_AABB_box.Enclose(mesh->OBB_box);
}

void C_Mesh::Update()
{
    UpdateBBData();
}
