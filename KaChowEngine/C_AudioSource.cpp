#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_AudioSource.h"

C_AudioSource::C_AudioSource(GameObject* owner, std::string uuid) : Component(nullptr, uuid)
{
	type = ComponentType::AUDIOSOURCE;
	sourceGameObject = owner;
	this->sourceID = audioId;
	//LOG("Audio Source ID:  %d", this->audioId);
	App->audio->RegisterGameObject(sourceID);
	volume = 100.0f; // Default volume level
}

C_AudioSource::~C_AudioSource()
{
	StopEvent();

	App->audio->UnregisterGameObject(sourceID);

	LOG("Destroying Audio Source Component");
}

void C_AudioSource::Update()
{
	//// Set source position
	//soundPos.SetPosition(sourceGameObject->mTransform->mPosition.x,
	//	sourceGameObject->mTransform->mPosition.y,
	//	sourceGameObject->mTransform->mPosition.z);

	//// Calculate forward and up vectors from mRotation (float3)
	//float3 forward = sourceGameObject->mTransform->mRotation.Normalized(); // Assuming mRotation represents the forward vector
	//float3 up = Cross(float3(0.0f, 1.0f, 0.0f), forward).Normalized(); // Calculate the up vector

	//// Set source orientation
	////soundPos.SetOrientation({ forward.x, forward.y, forward.z }, { up.x, up.y, up.z });

	//// Update Source Transform
	//AK::SoundEngine::SetPosition(sourceID, soundPos);

	App->audio->SetSourcePos(sourceGameObject, sourceID);
	//pilota
	//App->audio->CheckReverbGameObject(mParen->mTransform->mPosition);
}


void C_AudioSource::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio Source", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "Enabled: ");
		ImGui::SameLine();
		ImGui::Checkbox("##audioSourceEnabled", &active);

		ImGui::Spacing();
		ImGui::Spacing();

		PrintAudioList();

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

		/*if (ImGui::Button("Remove Component ##source", ImVec2(ImGui::GetWindowSize().x, 20.0f)))
			mParent->RemoveComponent(this);*/

		ImGui::PopStyleColor(3);

		ImGui::Spacing();

	}
}

void C_AudioSource::PlayEvent()
{
	playID = App->audio->PostEvent(audio.c_str(), sourceID);
	App->audio->SetRTPCValue("Volume", volume, sourceID);

}

void C_AudioSource::StopEvent() const
{
	App->audio->StopEvent(audio.c_str(), sourceID);

}

void C_AudioSource::ResumeEvent() const
{
	App->audio->ResumeEvent(audio.c_str(), sourceID);
	App->audio->SetRTPCValue("Volume", volume, sourceID);
}

void C_AudioSource::PauseEvent() const
{
	App->audio->PauseEvent(audio.c_str(), sourceID);

}

void C_AudioSource::PrintAudioList()
{

	if (ImGui::BeginCombo("##AudioClip", audio.c_str()))
	{
		for (int i = 0; i < App->audio->events.size(); i++)
		{
			if (ImGui::Selectable(App->audio->events[i].c_str()))
			{
				audio = App->audio->events[i];

			}
		}

		ImGui::EndCombo();
	}

	ImGui::Spacing();

	if (ImGui::Button("Play"))
	{
		if (!isPlaying)
		{
			PlayEvent();
			isPlaying = true;
		}

	}

	ImGui::SameLine();

	if (ImGui::Button("Stop"))
	{
		StopEvent();
		isPlaying = false;
	}

	ImGui::SameLine();

	if (ImGui::Button("Pause"))
	{
		PauseEvent();
		isPlaying = false;
	}

	ImGui::SameLine();

	if (ImGui::Button("Resume"))
	{
		ResumeEvent();
		isPlaying = true;
	}

	ImGui::Spacing();
	ImGui::Separator();

	if (ImGui::SliderFloat("Volume", &volume, 0.0f, 100.0f))
	{
		// Update the volume using Wwise API
		App->audio->SetRTPCValue("Volume", (AkRtpcValue)volume, sourceID);
	}

}