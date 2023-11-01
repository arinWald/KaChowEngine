#include "C_Mesh.h"
#include "Application.h"


C_Mesh::C_Mesh() : Component(nullptr)
{
	type = ComponentType::MESH;
	mesh = nullptr;
    showNormals = false;
}

C_Mesh::C_Mesh(GameObject* parent) : Component(nullptr)
{
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
        ImGui::SameLine();
        ImGui::Text("%d", mesh->num_index);
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