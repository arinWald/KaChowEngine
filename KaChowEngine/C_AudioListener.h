#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleAudio.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class C_AudioListener : public Component
{
public:
	C_AudioListener(GameObject* owner, std::string uuid);
	~C_AudioListener();

	void Update() override;

	void OnEditor() override;

	GameObject* listenerGameObject;

};