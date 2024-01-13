#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_AudioListener.h"

C_AudioListener::C_AudioListener(GameObject* owner, std::string uuid) : Component(nullptr, uuid)
{
	type = ComponentType::AUDIOLISTENER;
	listenerGameObject = owner;
	listenerId = audioId;
	LOG("Audio Listener ID:  %d", audioId);

	App->audio->RegisterGameObject(listenerId);
	App->audio->SetDefaultListener(listenerId);

}

C_AudioListener::~C_AudioListener()
{
	App->audio->RemoveDefaultListener(listenerId);

	App->audio->UnregisterGameObject(listenerId);

	LOG("Destroying AudioListener");
}

void C_AudioListener::Update()
{
	App->audio->SetListenerPos(listenerGameObject, listenerId);
}

void C_AudioListener::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio Listener", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "Enabled: "); ImGui::SameLine(); ImGui::Checkbox("##audioListenerEnabled", &active);

		ImGui::Spacing();
		ImGui::Spacing();

		if (active)
		{
			ImGui::TextColored(ImVec4(0, 255, 0, 255), "'%s' is now a LISTENER", mParent->name.c_str());
			App->audio->SetDefaultListener(listenerId);

		}
		else
		{
			ImGui::TextColored(ImVec4(255, 0, 0, 255), "'%s' is NOT a LISTENER", mParent->name.c_str());
			App->audio->RemoveDefaultListener(listenerId);

		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

		/*if (ImGui::Button("Remove Component ##listener", ImVec2(ImGui::GetWindowSize().x, 20.0f)))
			mParent->RemoveComponent(this);*/

		ImGui::PopStyleColor(3);

		ImGui::Spacing();

	}
}