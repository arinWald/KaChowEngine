#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ImGui/imgui.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	rootGameObject = nullptr;
	selectedGameObj = nullptr;
	draggedGameObject = nullptr;
	hoveredGameObj = nullptr;
}

bool ModuleScene::Init()
{
	return false;
}

bool ModuleScene::Start()
{

	rootGameObject = new GameObject(nullptr);
	rootGameObject->name = "Scene";


	App->geoLoader->LoadFile("Assets/Models/BakerHouse.fbx");

	App->camera->Position = float3(1.0f, 1.0f, 1.0f);
	App->camera->LookAt(float3(0, 0, 0));

	prova1 = new GameObject(rootGameObject);
	prova2 = new GameObject(prova1);
	prova3 = new GameObject(prova1);
	prova4 = new GameObject(rootGameObject);
	prova5 = new GameObject(prova4);
	prova6 = new GameObject(prova4);

	return true;
}

update_status ModuleScene::Update(float dt)
{
	bool ret = UPDATE_CONTINUE;

	

	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	delete rootGameObject;

	return true;
}

void ModuleScene::PrintHierarchy(GameObject* gameObject, int index)	
{
	ImGuiTreeNodeFlags flag_TNode = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_OpenOnArrow;

	bool isNodeOpen;

	if (gameObject->GetParent() == nullptr)
	{
		flag_TNode |= ImGuiTreeNodeFlags_DefaultOpen;
	}
	else
	{
		flag_TNode |= ImGuiTreeNodeFlags_OpenOnArrow;
	}

	if (gameObject == selectedGameObj) 
	{
		flag_TNode |= ImGuiTreeNodeFlags_Selected;
	}

	if (gameObject->mChildren.size() != 0)
		isNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)index, flag_TNode, gameObject->name.c_str(), index);

	else 
	{
		flag_TNode |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		ImGui::TreeNodeEx((void*)(intptr_t)index, flag_TNode, gameObject->name.c_str(), index);
		isNodeOpen = false;
	}


	if (gameObject->GetParent() != nullptr)
	{
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("GameObject", gameObject, sizeof(GameObject*));

			draggedGameObject = gameObject;
			ImGui::Text("Drag to");
			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
		{
			hoveredGameObj = gameObject;
			if (ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
			{
				selectedGameObj = gameObject;
			}
		}
	}

	if (ImGui::IsWindowHovered())
	{
		if (!ImGui::IsAnyItemHovered())
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
			{
				selectedGameObj = nullptr;
			}
		}
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject")) {

			/*if (!draggedGameObject->fixed || !hoveredGameObj->fixed) {
				draggedGameObject->SetNewParent(hoveredGameObj);
			}*/
			draggedGameObject->SetNewParent(hoveredGameObj);
		}
		ImGui::EndDragDropTarget();
	}

	if (isNodeOpen)
	{
		if (!gameObject->mChildren.empty()) {
			for (unsigned int i = 0; i < gameObject->mChildren.size(); i++)
			{
				PrintHierarchy(gameObject->mChildren[i], i);
			}
		}
		ImGui::TreePop();
	}
				
}

GameObject* ModuleScene::CreateGameObject(GameObject* parent)
{
	GameObject* newGameObject = new GameObject(parent);
	return nullptr;
}
