#pragma once
#include "Globals.h"
#include "Component.h"
#include "Imgui/imgui.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "MathGeoLib/include/Math/float4x4.h"

class GameObject;
class Component;

class C_Camera : public Component
{
public:

	C_Camera();
	C_Camera(GameObject* owner);
	~C_Camera();

	void SetCam();
	void LookAt(const float3& target);
	void GenBuffer();

	float* GetViewMatrix();
	float* GetProjectionMatrix();

	void OnEditor() override;

	void CameraTransform();

	Frustum FrustumCam;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;

	unsigned int cameraBuffer;
	unsigned int frameBuffer;
	int FOV = 60.0f;

private:

	unsigned int renderObjBuffer;
};