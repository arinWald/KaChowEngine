#pragma once
#include "Globals.h"
#include "GameObject.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Component.h"
#include "ImGui/imgui.h"
#include <string>

class GameObject;
class Component;

class C_Transform : public Component
{
public:
	C_Transform(std::string uuid);
	C_Transform(GameObject* parent, std::string uuid);
	~C_Transform();

	void OnEditor() override;

	virtual void Enable();
	virtual void Disable();
	/*virtual Update();*/

	float4x4 getGlobalMatrix();
	float4x4 getLocalMatrix();

	void resetMatrix();

	float3 getPosition(bool globalPosition = false);
	void setPosition(float3 pos);
	float3 getRotation();
	void setRotation(float3 rotation);
	float3 getScale();
	void setScale(float3 scale);

	void SetTransformMatrixW(float4x4 matrix);

	//// Pilota
	//void SetTransfoMatrix(float3 position, Quat rotation, float3 scale);

	void calculateMatrix();

	float3 mPosition;
	float3 mScale;
	float3 mRotation;

	// Posarli matriux local del parent
	float4x4 mGlobalMatrix;
	float4x4 mLocalMatrix;
};