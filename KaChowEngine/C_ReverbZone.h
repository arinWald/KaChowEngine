#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleAudio.h"
#include "Globals.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;


class C_ReverbZone : public Component
{
public:
	C_ReverbZone(GameObject* owner, std::string uuid);

	~C_ReverbZone();

	void ResizeReverbZone();

	inline AABB GetReverbZoneAABB() const { return reverbBoxZone; }
	inline std::string GetReverbBusName() const { return busReverb; }

	void CompileBuffers();
	void Draw();

	void Update() override;

	void OnEditor() override;

	GameObject* reverbZoneObject;

	uint reverbId;

private:

	AABB reverbBoxZone;
	string busReverb;

	// Show reverb zone for debug purposes
	uint reverbIndex = 24;
	uint reverbVertex = sizeof(float3) * 8;
	float* vertex = nullptr;
	uint* index = nullptr;
};