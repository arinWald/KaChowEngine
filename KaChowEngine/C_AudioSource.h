#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class C_AudioSource : public Component
{
public:
	C_AudioSource(GameObject* gameObject, std::string uuid);
	~C_AudioSource();

	void Update() override;

	void OnEditor() override;

	void PrintAudioList();

	void PlayEvent();
	void StopEvent() const;
	void PauseEvent() const;
	void ResumeEvent() const;

	bool toPlayEvent = false;
	unsigned toPlayEventId = 0;

	unsigned int playID;
	float volume;
	bool playOnAwake;
	bool isPlaying = false;

	GameObject* sourceGameObject;
	uint sourceID;

	std::string audio;

};