#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Geometry/Frustum.h"

class C_Camera;
class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	//void Look(const float3& Position, const float3& Reference, bool RotateAroundReference = false);
	//void LookAt(const float3& Spot);
	//void Move(const float3& Movement);
	//float* GetViewMatrix();
	//float* GetProjectionMatrix();

	//void FocusCameraToSelectedObject();
	//void RotationAroundCamera(float dt);
	//void OrbitSelectedObject(float dt);
	//float3 RotateVector(const float3& u, float angle, const float3& v);

	//unsigned int cameraBuffer;
	//unsigned int frameBuffer;

	C_Camera* sceneCam;
//
//private:
//
//	unsigned int renderObjBuffer;
//
//public:
//
//	Frustum FrustumCam;
//	float4x4 viewMatrix;
//	float4x4 projectionMatrix;
//
//private:
//
//	//float4x4 ViewMatrix;
};