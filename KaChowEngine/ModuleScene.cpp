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
	name = "Scene";
}

bool ModuleScene::Init()
{
	return false;
}

bool ModuleScene::Start()
{

	rootGameObject = new GameObject(nullptr);
	rootGameObject->name = "Scene";


	bakerHouse = App->geoLoader->LoadFile("Assets/Models/BakerHouse.fbx");
	bakerHouse->name = "BakerHouse";

	street = App->geoLoader->LoadFile("Assets/Models/scene.DAE");
	street->name = "Street";
	street->mTransform->mRotation.x = -90;
	street->mTransform->calculateMatrix();


	// Game camera at start
	currentGameCamera = new GameObject(rootGameObject);
	currentGameCamera->name = "Game Camera";
	C_Camera* cameraComponent = new C_Camera(UUIDGenerator::Generate());
	currentGameCamera->AddComponent(cameraComponent);
	currentGameCamera->mTransform->setPosition({ 0, 5, 0 });
	//currentGameCamera->mTransform->setRotation({ -50, 35, 0 });

	f = 0;

	return true;
}

update_status ModuleScene::Update(float dt)
{
	bool ret = UPDATE_CONTINUE;

	rootGameObject->Update();

	//currentGameCamera->mTransform->mPosition += {.1f * App->dtGame, .1f * App->dtGame, .1f * App->dtGame};

	rotation = 1;
	f += App->dtGame;

	if (App->gameState == GameState::PLAY)
	{
		if (f > 0.03f) {
			currentGameCamera->mTransform->mRotation.y += rotation;
			currentGameCamera->mTransform->calculateMatrix();
			if (currentGameCamera->mTransform->mRotation.y == 360) {
				currentGameCamera->mTransform->mRotation.y = 0;
			}
			f = 0.0f;
		}
	}
	else
	{
		currentGameCamera->mTransform->mRotation.y = 0;
		currentGameCamera->mTransform->calculateMatrix();
	}

	// Gizmos controls
	if (App->input->GetKey(SDL_SCANCODE_W)) App->camera->operation = ImGuizmo::OPERATION::TRANSLATE;
	else if (App->input->GetKey(SDL_SCANCODE_E)) App->camera->operation = ImGuizmo::OPERATION::ROTATE;
	else if (App->input->GetKey(SDL_SCANCODE_R)) App->camera->operation = ImGuizmo::OPERATION::SCALE;

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

void ModuleScene::SetGameObjectSelected(GameObject* gameObject)
{
	if (gameObject == nullptr) {
		selectedGameObj = nullptr;
		return;
	}

	selectedGameObj = gameObject;
}

GameObject* ModuleScene::CreateGameObject(GameObject* parent)
{
	GameObject* newGameObject = new GameObject(parent);
	return newGameObject;
}
