#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_ReverbZone.h"
#include "ModuleAudio.h"

C_ReverbZone::C_ReverbZone(GameObject* owner, std::string uuid) : Component(nullptr, uuid)
	//GameObject* own, TransformComponent* trans) : transform(trans), Component(), busReverb("Reverb"), vbo(nullptr), ebo(nullptr)
{
	reverbZoneObject = owner;
	type = ComponentType::REVERBZONE;

	reverbZoneObject = owner;
	reverbId = audioId;

	App->audio->RegisterGameObject(reverbId);

	ResizeReverbZone();
	CompileBuffers();
	App->audio->AddReverbZone(this);
}

C_ReverbZone::~C_ReverbZone()
{
	App->audio->DeleteReverbZone(this);

	//pilota
	RELEASE(vbo);
	RELEASE(ebo);
}

void C_ReverbZone::ResizeReverbZone()
{
	reverbBoxZone.SetNegativeInfinity();
	reverbBoxZone.SetFromCenterAndSize(reverbZoneObject->mTransform->getPosition(), float3(5.0f, 5.0f, 5.0f));

}

void C_ReverbZone::CompileBuffers()
{
	// Configure buffers
	float3 corners[8];
	reverbBoxZone.GetCornerPoints(corners);
	unsigned int indices[24] =
	{
		0,1,
		1,3,
		3,2,
		2,0,

		1,5,
		4,6,
		7,3,

		6,7,
		6,2,

		7,5,
		4,5,

		4,0
	};

	if (vbo)
	{
		vbo->Unbind();
		RELEASE(vbo);
	}
	ebo = new IndexBuffer(indices, 24);
	vbo = new VertexBuffer(corners, sizeof(float3) * 8);
	ebo->Unbind();
	vbo->Unbind();
}

void C_ReverbZone::Draw()
{
	glPushMatrix();

	glMultMatrixf(reverbZoneObject->mTransform->getGlobalMatrix().ptr());
	glEnableClientState(GL_VERTEX_ARRAY);


	glGenBuffers(1, (GLuint*)&(reverbVertex));
	glBindBuffer(GL_ARRAY_BUFFER, reverbVertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * reverbVertex * VERTEX_ARGUMENTS, vertex, GL_STATIC_DRAW);

	glVertexPointer(3, GL_FLOAT, 0, NULL);
	
	glGenBuffers(1, (GLuint*)&(reverbIndex));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, reverbIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * reverbIndex, index, GL_STATIC_DRAW);

	glLineWidth(2.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glDrawElements(GL_LINES, reverbIndex, GL_UNSIGNED_INT, NULL);
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

void C_ReverbZone::Update()
{
	float3 position = reverbZoneObject->mTransform->mPosition;

	AkSoundPosition reverbPos;
	reverbPos.SetOrientation({ 0,0,-1 }, { 0,1,0 });
	reverbPos.SetPosition(position.x, position.y, position.z);

	AK::SoundEngine::SetPosition(reverbId, reverbPos);
}

void C_ReverbZone::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio Reverb Zone"))
	{

	}
}