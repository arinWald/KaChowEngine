#pragma once
#include "Globals.h"
#include "GameObject.h"
#include "Component.h"
#include "Glew/include/glew.h"
#include "ImGui/imgui.h"

class GameObject;
class Component;

class C_Camera : public Component
{
public:
	C_Camera();
	C_Camera(GameObject* parent);
	~C_Camera();

	void OnEditor() override;

	void SetCam();
	void GenFrameBuffer();

	float* GetViewMatrix();
	float* GetProjectMatrix();

	void LookAt(const float3& target);
	void TransformCam();
	bool HasBoundingBoxIsnide(Mesh* refBox);

	Frustum frustrumCamera;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;

	// pilota
	int printCount;

	int fov;

	uint frameBuffer;
	uint camBuffer;

	virtual void Enable();
	virtual void Disable();
	//virtual Update();

	// Pilota
	uint renderObjBuffer;

};