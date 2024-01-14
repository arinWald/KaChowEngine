#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ImGui/imgui.h"
#include "C_Transform.h"
#include "C_Material.h"

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
	jsonFile.FileToValue("scene.json");
	return false;
}

bool ModuleScene::Start()
{

	rootGameObject = new GameObject(nullptr);
	rootGameObject->name = "Scene";

	backgroundAudioSource = new GameObject(rootGameObject);
	backgroundAudioSource->name = "Background Music Source";
	C_AudioSource* bgAudioSource = new C_AudioSource(backgroundAudioSource, UUIDGenerator::Generate());
	backgroundAudioSource->AddComponent(bgAudioSource);

	spatialAudioSource = App->geoLoader->LoadFile("Assets/Models/cube.fbx");
	spatialAudioSource->name = "Spatial Audio Source";
	C_AudioSource* spaAudioSource = new C_AudioSource(spatialAudioSource, UUIDGenerator::Generate());
	spatialAudioSource->AddComponent(spaAudioSource);
	spatialAudioSource->mTransform->setPosition({ 0, 0, 10 });

	bakerHouse = App->geoLoader->LoadFile("Assets/Models/BakerHouse.fbx");
	bakerHouse->name = "BakerHouse (Listener)";
	C_AudioListener* audioListenerComponent = new C_AudioListener(bakerHouse, UUIDGenerator::Generate());
	bakerHouse->AddComponent(audioListenerComponent);

	// Game camera at start
	currentGameCamera = new GameObject(rootGameObject);
	currentGameCamera->name = "Game Camera";
	C_Camera* cameraComponent = new C_Camera(UUIDGenerator::Generate());
	currentGameCamera->AddComponent(cameraComponent);
	currentGameCamera->mTransform->setPosition({ 0, 5, -10 });
	//currentGameCamera->mTransform->setRotation({ -50, 35, 0 });

	street = App->geoLoader->LoadFile("Assets/Models/scene.DAE");
	street->name = "Street";
	street->mTransform->mRotation.x = -90;
	street->mTransform->calculateMatrix();
	
	// Set Play on engine start
	// Last chapuza 
	App->gameState = GameState::PLAY;

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
		// Spatial Audio Cube Movement
		if (f > 0.03f) {
			spatialAudioSource->mTransform->mPosition.x += 0.1f;

			if (spatialAudioSource->mTransform->mPosition.x > 10.0f) {
				spatialAudioSource->mTransform->mPosition.x = -10.0f;
			}

			spatialAudioSource->mTransform->calculateMatrix();

			f = 0.0f;
		}
	}


	// Gizmos controls
	if (App->input->GetKey(SDL_SCANCODE_W)) App->camera->operation = ImGuizmo::OPERATION::TRANSLATE;
	else if (App->input->GetKey(SDL_SCANCODE_E)) App->camera->operation = ImGuizmo::OPERATION::ROTATE;
	else if (App->input->GetKey(SDL_SCANCODE_R)) App->camera->operation = ImGuizmo::OPERATION::SCALE;

	UpdateGameObjects();

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

bool ModuleScene::SaveScene()
{
	LOG("Saving scene");

	rootFile = jsonFile.GetRootValue();

	JsonParser scene = jsonFile.SetChild(rootFile, "GameObjects");

	SaveGameObjects(rootGameObject, scene.SetChild(scene.GetRootValue(), rootGameObject->name.c_str()));

	jsonFile.SerializeFile(rootFile, "scene.json");
	saveSceneRequest = false;
	return true;
}

void ModuleScene::SaveGameObjects(GameObject* parentGO, JsonParser& node) 
{
	std::string num;
	JsonParser& child = node;
	C_Transform* transform;
	float4x4 localTransform;

	node.SetNewJsonString(node.ValueToObject(node.GetRootValue()), "name", parentGO->name.c_str());
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "Deleting", parentGO->deleteGameObject);


	JsonParser components = node.SetChild(node.GetRootValue(), "components");
	JsonParser tmp = node;
	for (size_t i = 0; i < parentGO->mComponents.size(); i++)
	{
		// Create Child of component
		num = "Component " + std::to_string(i);
		//num += FormatComponentType(parentGO, i);



		tmp = components.SetChild(components.GetRootValue(), num.c_str());

		tmp.SetNewJsonNumber(tmp.ValueToObject(tmp.GetRootValue()), "Type", (int)parentGO->mComponents.at(i)->type);

		switch ((ComponentType)parentGO->mComponents.at(i)->type)
		{
		case ComponentType::TRANSFORM:
			num = "";
			transform = (C_Transform*)(parentGO->mComponents.at(i));
			localTransform = transform->getLocalMatrix();
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					/* if (i == 0 && j == 0)num += std::to_string(localTransform.At(i, j));
					 else num += "," + std::to_string(localTransform.At(i, j));*/

					num += std::to_string(localTransform.At(i, j));
					num += ",";
				}
			}

			tmp.SetNewJsonString(tmp.ValueToObject(tmp.GetRootValue()), "LocalTransform", num.c_str());

			break;

		case ComponentType::MESH:

			tmp.SetNewJsonString(tmp.ValueToObject(tmp.GetRootValue()), "Mesh", parentGO->name.c_str());

			break;

		case ComponentType::MATERIAL:

			C_Material* componentMaterial;
			componentMaterial = (C_Material*)(parentGO->mComponents.at(i));

			tmp.SetNewJsonString(tmp.ValueToObject(tmp.GetRootValue()), "Material", componentMaterial->texture_path.c_str());


			break;

		default:
			break;

		}
		parentGO->mComponents.at(i)->type;
	}

	for (size_t i = 0; i <= parentGO->mChildren.size(); i++)
	{

		num = "Child " + std::to_string(i);

		if (parentGO->mChildren.size() > i) {
			SaveGameObjects(parentGO->mChildren[i], child.SetChild(child.GetRootValue(), num.c_str()));
		}
	}
}

bool ModuleScene::LoadScene()
{
	LOG("Loading configurations");


	rootFile = jsonFile.GetRootValue();

	rootGO = jsonFile.GetChild(rootFile, "GameObjects");

	LoadGameObject(rootGO.GetChild(rootGO.GetRootValue(), "Scene"));

	loadSceneRequest = false;

	return false;
}

void ModuleScene::LoadComponents(JsonParser& parent, std::string num, GameObject* gamObj)
{

	C_Transform* transform;
	C_Mesh* meshRender;
	Mesh* mesh;
	C_Material* material;
	float3 size = float3::one;
	LOG("Loading Components \n");
	std::string debugPath;
	std::string debugUID;

	JsonParser components = parent.GetChild(parent.GetRootValue(), "components");
	JsonParser tmp = components;

	std::string pos;

	for (int i = 0; i < 4; i++)
	{
		num = "Component " + std::to_string(i);
		LOG((std::string("Loading ") + num).c_str());

		if (components.ExistChild(components.GetRootValue(), num.c_str()))
		{
			tmp = components.GetChild(components.GetRootValue(), num.c_str());
			switch ((ComponentType)(int)tmp.JsonValToNumber("Type"))
			{
			case ComponentType::TRANSFORM:
				gamObj->mTransform->mLocalMatrix = strMatrixToF4x4(tmp.ValueToString("LocalTransform"));

				LOG(gamObj->mTransform->getLocalMatrix().ToString().c_str());
				break;
			case ComponentType::MESH:

				break;
			case ComponentType::MATERIAL:
				break;
			}
		}
		else
		{
			break;
		}
	}
}

GameObject* ModuleScene::LoadGameObject(JsonParser parent, GameObject* father)
{
	std::string num;
	std::string convert;

	std::string name = parent.ValueToString("name");
	GameObject* gamObj = new GameObject();
	gamObj->name = name;
	gamObj->SetDeletion(parent.JsonValToBool("isTimeToDelete"));
	
	LoadComponents(parent, num, gamObj);
	int count = 0;
	num = "Child " + std::to_string(count);
	while (parent.ExistChild(parent.GetRootValue(), num.c_str()))
	{
		gamObj->SetNewParent(LoadGameObject(parent.GetChild(parent.GetRootValue(), num.c_str()), gamObj));
		++count;
		num = "Child " + std::to_string(count);
	}


	return gamObj;
}

void ModuleScene::UpdateGameObjects()
{
	if (saveSceneRequest)SaveScene();
	if (loadSceneRequest)LoadScene();
}

const char* ModuleScene::FormatComponentType(GameObject* parentGO, const size_t& i)
{
	switch ((ComponentType)parentGO->mComponents.at(i)->type)
	{
	case ComponentType::TRANSFORM:

		return " Transform";
		break;

	case ComponentType::MESH:

		return " Mesh";
		break;

	case ComponentType::MATERIAL:

		return " Material";
		break;

	default:
		break;

	}
}

float4x4 ModuleScene::strMatrixToF4x4(const char* convert)
{
	std::string text = convert;
	std::string delimiter = ",";
	std::vector<float> floatArray{};

	size_t pos = 0;
	while ((pos = text.find(delimiter)) != std::string::npos) {
		floatArray.push_back(stof(text.substr(0, pos)));
		text.erase(0, pos + delimiter.length());
	}


	float4x4 matrix;
	int count = 0;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{

			matrix.At(i, j) = floatArray.at(count);
			++count;
		}

	return matrix;
}