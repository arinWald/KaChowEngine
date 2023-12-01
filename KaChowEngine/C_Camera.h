#pragma once
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class C_Camera : public Component
{
public:
	C_Camera();
	C_Camera(GameObject owner);
	~C_Camera();

	void PrintInspector();
	void Update();

	void Look(const float3& Position, const float3& Reference);
	void LookAt(const float3& Spot);
	void Move(const float3& Movement);
	float* GetViewMatrix();
	float* GetProjetionMatrix();

	Frustum frustum;
	float3 reference;

private:
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};