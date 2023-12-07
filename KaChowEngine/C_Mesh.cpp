#include "C_Mesh.h"
#include "Application.h"


C_Mesh::C_Mesh(std::string uuid) : Component(nullptr, uuid)
{
	type = ComponentType::MESH;
    showNormals = false;
    mParent = nullptr;
}

C_Mesh::C_Mesh(GameObject* parent, std::string uuid) : Component(parent, uuid)
{
    type = ComponentType::MESH;
    showNormals = false;
    mParent = parent;
}

C_Mesh::~C_Mesh()
{
    for (int i = 0; i < meshes.size(); i++)
    {
        App->geoLoader->DestroyMesh(meshes[i]);
    }
    meshes.clear();
}

void C_Mesh::Update()
{
    UpdateBBData();
}

void C_Mesh::UpdateBBData()
{
    for (int i = 0; i < meshes.size(); i++)
    {
        meshes[i]->OBB_box = meshes[i]->AABB_box;
        meshes[i]->OBB_box.Transform(meshes[i]->owner->mTransform->getGlobalMatrix().Transposed());
        meshes[i]->Global_AABB_box.SetNegativeInfinity();
        meshes[i]->Global_AABB_box.Enclose(meshes[i]->OBB_box);

    }
}

void C_Mesh::OnEditor()
{
    if (ImGui::CollapsingHeader("Mesh"))
    {
        if (meshes.empty()) return;
        for (int i = 0; i < meshes.size(); i++)
        {
            ImGui::LabelText("##%f", "Number of vertex:");
            ImGui::SameLine();
            ImGui::Text("%d", meshes[i]->num_vertex);
            ImGui::LabelText("##%f", "Number of index:");
        }

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