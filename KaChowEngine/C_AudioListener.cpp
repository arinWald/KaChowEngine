#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_AudioListener.h"

C_AudioListener::C_AudioListener(GameObject* owner, std::string uuid) : Component(nullptr, uuid, audioId)
{
	listenerGameObject = owner;


}

C_AudioListener::~C_AudioListener()
{

}

void C_AudioListener::Update()
{
}

void C_AudioListener::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio Listener", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "Enabled: ");
		ImGui::SameLine();
		ImGui::Checkbox("##audioListenerEnabled", &active);

		ImGui::Spacing();
		ImGui::Spacing();

		if (active)
			ImGui::TextColored(ImVec4(0, 255, 0, 255), "'%s' is now a LISTENER", mParent->name.c_str());
		else
			ImGui::TextColored(ImVec4(255, 0, 0, 255), "'%s' is NOT a LISTENER", mParent->name.c_str());

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

		ImGui::PopStyleColor(3);

		ImGui::Spacing();

	}
}