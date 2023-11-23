#include "GameObject.h"
#include "OurPrimitive.h"

GameObject::GameObject()
{
	name = "GameObject";
	mParent = nullptr;
	mTransform = new C_Transform(mParent);
	type = ShapeType::NONE;
	deleteGameObject = false;

	mComponents.push_back(mTransform);
}

GameObject::~GameObject()
{

	//for (size_t i = 0; i < mComponents.size(); ++i)
	//{
	//	delete mComponents[i];
	//	//mComponents[i] = nullptr;
	//}
	//mComponents.clear();


	//if (mParent != nullptr) {
	//	for (size_t i = 0; i < mParent->mChildren.size(); ++i)
	//	{
	//		if (mParent->mChildren[i] == this) {
	//			mParent->mChildren.erase(mParent->mChildren.begin() + i);
	//			mParent = nullptr;
	//		}
	//		break;
	//	}
	//}
	//mTransform = nullptr;


	//while (!mChildren.empty())
	//{
	//	delete mChildren[0];
	//}
	//mChildren.clear();


	/*name = "";*/
	mTransform = nullptr;

	if (deleteGameObject && mParent != nullptr) {
		mParent->DeleteChild(this);
	}

	for (size_t i = 0; i < mComponents.size(); ++i)
	{
		delete mComponents[i];
		mComponents[i] = nullptr;
	}

	for (size_t i = 0; i < mChildren.size(); ++i)
	{
		delete mChildren[i];
		mChildren[i] = nullptr;
	}

	/*for (size_t i = mComponents.size(); i >= 0; --i)
	{
		delete mComponents[i];
		mComponents[i] = nullptr;
	}

	for (size_t i = mChildren.size(); i >= 0; --i)
	{
		delete mChildren[i];
		mChildren[i] = nullptr;
	}*/

	//mComponents.clear();
}

GameObject::GameObject(GameObject* parent)
{
	name = "GameObject";
	this->mParent = parent;

	if (parent != nullptr)
	{
		parent->mChildren.push_back(this);
	}

	mTransform = new C_Transform(mParent);
	
	type = ShapeType::NONE;

	deleteGameObject = false;

	mComponents.push_back(mTransform);
}

void GameObject::AddComponent(Component* component)
{
	mComponents.push_back(component);
	component->mParent = this;
}


void GameObject::Update()
{
	// No tots els children o components tenen un update
	for (size_t i = 0; i < mChildren.size(); ++i)
	{
		mChildren[i]->Update();
	}

	for (size_t i = 0; i < mComponents.size(); ++i)
	{
		mComponents[i]->Update();
	}
}

GameObject* GameObject::GetParent()
{
	return mParent;
}

C_Mesh* GameObject::GetMeshComponent()
{
	for (size_t i = 0; i < mComponents.size(); i++)
	{
		if (mComponents[i]->type == ComponentType::MESH)
		{
			return (C_Mesh*)mComponents[i];
		}
	}
	return nullptr;
}

C_Material* GameObject::GetMaterialComponent()
{
	for (size_t i = 0; i < mComponents.size(); i++)
	{
		if (mComponents[i]->type == ComponentType::MATERIAL)
		{
			return (C_Material*)mComponents[i];
		}
	}
	return nullptr;
}

C_Camera* GameObject::GetCameraComponent()
{
	for (int i = 0; i < mComponents.size(); i++) {

		if (mComponents[i]->type == ComponentType::CAMERA)
		{
			return (C_Camera*)mComponents[i];
		}
	}
}

bool GameObject::IsChildOf(GameObject* gameObject)
{
	if (gameObject == this)
	{
		return true;
	}
	if (gameObject->mChildren.empty())
	{
		return false;
	}
	for (size_t i = 0; i < gameObject->mChildren.size(); i++)
	{
		if (IsChildOf(gameObject->mChildren[i])) return true;
	}
	return false;
}

void GameObject::DeleteChild(GameObject* child)
{
	for (int i = 0; i < mChildren.size(); i++) {
		if (mChildren[i] == child) {
			mChildren.erase(mChildren.begin() + i);
			child->mParent = nullptr;
		}
	}
}

void GameObject::PrintOnInspector()
{
	char aux[255] = { ' ' };

	if (mParent != nullptr)
	{
		strcpy(aux, this->name.c_str());

		ImGui::BulletText("Name:");
		ImGui::SameLine();

		ImGui::InputText("##Name", aux, 255, ImGuiInputTextFlags_EnterReturnsTrue);

		if (ImGui::IsKeyDown(ImGuiKey_Enter))
			name = aux;

		if (ImGui::Button("Delete")) {

			deleteGameObject = true;
			delete App->scene->selectedGameObj;
			App->scene->selectedGameObj = nullptr;
			
		}

		for (size_t i = 0; i < mComponents.size(); i++)
		{
			ImGui::Separator();

			mComponents[i]->OnEditor();
		}

		ImGui::Separator();
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");

		ImGui::Text("");
		ImGui::SameLine(ImGui::GetWindowWidth() / 6);
	}
}

bool GameObject::SetNewParent(GameObject* newParent)
{
	if (mParent != nullptr) {
		if (newParent->IsChildOf(this)) return false;

		mParent->DeleteChild(this);
	}

	mParent = newParent;
	newParent->mChildren.push_back(this);

	return true;
}

bool GameObject::AddThisChild(GameObject* gameObject)
{
	if (IsChildOf(gameObject))
	{
		return false;
	}
	gameObject->mParent = this;
	mChildren.push_back(gameObject);
	return true;
}
