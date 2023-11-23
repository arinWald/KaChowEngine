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
	void GenBuffer();

	float* GetViewMatrix();
	float* GetProjectionMatrix();

	void Inspector();

	Frustum FrustumCam;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;

	unsigned int cameraBuffer;
	unsigned int frameBuffer;

private:

	unsigned int renderObjBuffer;
};