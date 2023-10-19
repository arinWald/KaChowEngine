#pragma once
#include "Globals.h"
#include "GameObject.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Component.h"

class GameObject;
class Component;

class C_Transform : public Component
{
public:
	C_Transform();
	C_Transform(GameObject* parent);
	~C_Transform();

	void OnEditor() override;

	virtual void Enable();
	virtual void Disable();
	/*virtual Update();*/

	void SetTransfoMatrix(float3 position, float3 rotation, float3 scale);

	float3 mPosition;
	float3 mScale;
	float3 mRotation;
	Quat mRotation;

	float4x4 mGlobalMatrix;
	float4x4 mLocalMatrix;
};