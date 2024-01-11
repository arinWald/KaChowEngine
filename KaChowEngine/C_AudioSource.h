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
	C_AudioSource(std::string uuid);
	C_AudioSource(GameObject* gameObject, std::string uuid);
	~C_AudioSource();

	void Update() override;

	void OnEditor() override;

};