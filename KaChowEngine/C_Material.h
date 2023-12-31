#pragma once
#include "Globals.h"
#include "GameObject.h"
#include "Component.h"
#include "Glew/include/glew.h"
#include "ImGui/imgui.h"
#include <string>

class GameObject;
class Component;

enum CurrentTextureType
{
	TEXTURE,
	NONE,
	CHECKERS,
};

class C_Material : public Component
{
public:
	C_Material(std::string uuid);
	C_Material(GameObject* parent, std::string uuid);
	~C_Material();

	GLuint textureID;

	void OnEditor() override;
	void SetTexture(const char* file_path);
	void ResetTexture();
	void UpdateMeshTexture();

	CurrentTextureType currentTexture;

	virtual void Enable();
	virtual void Disable();
	/*virtual Update();*/

	std::string texture_path;


};