#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "ImGuizmo/ImGuizmo.h"

class C_Camera;

enum class CamStates {

	NORMAL,
	FLYING,
	FOCUSED,
};


class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	float3 SelectedObjectPos();

private:

	void MouseRotation(float dx, float dy, float sensitivity);

public:

	ImGuizmo::OPERATION operation;
	ImGuizmo::MODE mode;

	float3 X, Y, Z, Position, Reference;

	C_Camera* camera;
	CamStates camState;
	int typeCameraSelected;
};