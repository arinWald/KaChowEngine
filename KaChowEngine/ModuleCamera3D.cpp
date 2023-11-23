﻿#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "MathGeoLib/include/Math/float3.h"
#include "C_Transform.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	/*X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 10.0f, 5.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);
	ViewMatrix.SetIdentity();

	CalculateViewMatrix();*/

	FrustumCam.type = FrustumType::PerspectiveFrustum;
	FrustumCam.nearPlaneDistance = 0.1f;
	FrustumCam.farPlaneDistance = 500.f;
	FrustumCam.front = float3::unitZ;
	FrustumCam.up = float3::unitY;

	FrustumCam.verticalFov = 60.0f * DEGTORAD;
	FrustumCam.horizontalFov = 2.0f * atanf(tanf(FrustumCam.verticalFov / 2.0f) * 1.7f);

	FrustumCam.pos = float3(0, 0, -10);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	float speed = 3.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) FrustumCam.pos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) FrustumCam.pos.y -= speed;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) FrustumCam.pos -= FrustumCam.front * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) FrustumCam.pos += FrustumCam.front * speed;


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) FrustumCam.pos -= FrustumCam.WorldRight() * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) FrustumCam.pos += FrustumCam.WorldRight() * speed;



	//// Implement a debug camera with keys and mouse
	//// Now we can make this movememnt frame rate independant!

	//float3 newPos(0, 0, 0);
	//float speed = 3.0f * dt;
	//if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	//	speed = 8.0f * dt;

	//if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	//{
	//	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	//	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;

	//	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	//	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	//	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos += Y * speed;
	//	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos -= Y * speed;

	//	RotationAroundCamera(dt);
	//}

	//Position += newPos;
	//Reference += newPos;

	//if ((App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) && App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_IDLE) {

	//	Reference = float3(0.0f, 0.0f, 0.0f);
	//	LookAt(Reference);

	//}
	//else {


	//	Reference = Position;

	//}

	///*newPos -= App->input->GetMouseZ() * Z;

	//Position += newPos;
	//Reference += newPos;

	//Reference = Position;

	//OrbitSelectedObject(dt);

	//LookAt(Reference);*/

	//// Recalculate matrix -------------
	//CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
//void ModuleCamera3D::Look(const float3& Position, const float3& Reference, bool RotateAroundReference)
//{
//	this->Position = Position;
//	this->Reference = Reference;
//
//	Z = (Position - Reference).Normalized();
//	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
//	Y = Z.Cross(X);
//
//	if (!RotateAroundReference)
//	{
//		this->Reference = this->Position;
//		this->Position += Z * 0.05f;
//	}
//
//	CalculateViewMatrix();
//}
//
//// -----------------------------------------------------------------
//void ModuleCamera3D::LookAt(const float3& Spot)
//{
//	Reference = Spot;
//
//	Z = (Position - Reference).Normalized();
//	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
//	Y = Z.Cross(X);
//
//	CalculateViewMatrix();
//}
//
//
//// -----------------------------------------------------------------
//void ModuleCamera3D::Move(const float3& Movement)
//{
//	Position += Movement;
//	Reference += Movement;
//
//	CalculateViewMatrix();
//}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{

	viewMatrix = FrustumCam.ViewMatrix();

	viewMatrix.Transpose();

	return viewMatrix.ptr();
}

float* ModuleCamera3D::GetProjectionMatrix()
{

	projectionMatrix = FrustumCam.ProjectionMatrix();

	projectionMatrix.Transpose();

	return projectionMatrix.ptr();
}


// ALL CODE BELOW IS NOT BEING USED RIGHT NOW
//void ModuleCamera3D::FocusCameraToSelectedObject()
//{
//
//	float3 focusObjectPosition;
//	//Get the GameObject selected in hierarchy
//	if (App->scene->selectedGameObj	!= nullptr)
//	{
//		focusObjectPosition = App->scene->selectedGameObj->mTransform->getPosition();
//		LookAt(focusObjectPosition);
//	}
//
//}
//
//void ModuleCamera3D::OrbitSelectedObject(float dt)
//{
//	float3 pivot = float3(0, 0, 0);
//	GameObject* gameObject = App->scene->selectedGameObj;
//
//	float3 posGO = { 0, 0, 0 };
//
//	if (gameObject != nullptr)posGO = gameObject->mTransform->getPosition();
//
//	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
//	{
//		int dx = -App->input->GetMouseXMotion();
//		int dy = -App->input->GetMouseYMotion();
//		float Sensitivity = .5f * dt;
//
//		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
//		{
//
//			if (gameObject != nullptr)
//			{
//				pivot = float3(posGO.x, posGO.y, posGO.z);
//			}
//			else
//			{
//				return;
//			}
//		}
//		else
//		{
//			return;
//		}
//
//		Position -= pivot;
//
//		if (dx != 0)
//		{
//			float DeltaX = (float)dx * Sensitivity;
//
//			X = RotateVector(X, DeltaX, float3(0.0f, 1.0f, 0.0f));
//			Y = RotateVector(Y, DeltaX, float3(0.0f, 1.0f, 0.0f));
//			Z = RotateVector(Z, DeltaX, float3(0.0f, 1.0f, 0.0f));
//		}
//
//		if (dy != 0)
//		{
//			float DeltaY = (float)dy * Sensitivity;
//
//			Y = RotateVector(Y, DeltaY, X);
//			Z = RotateVector(Z, DeltaY, X);
//
//			if (Y.y < 0.0f)
//			{
//				Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
//				Y = Cross(Z, X);
//
//			}
//		}
//		Position = pivot + Z * Length(Position);
//		Reference = pivot;
//
//	}
//
//}
//
//void ModuleCamera3D::RotationAroundCamera(float dt)
//{
//	int dx = -App->input->GetMouseXMotion();
//	int dy = -App->input->GetMouseYMotion();
//
//	float Sensitivity = .5f * dt;
//
//	Position -= Reference;
//
//	if (dx != 0)
//	{
//		float DeltaX = (float)dx * Sensitivity;
//		
//		float3 rotationAxis(0.0f, 1.0f, 0.0f);
//		Quat rotationQuat = Quat::RotateAxisAngle(rotationAxis, DeltaX);
//
//		X = rotationQuat * X;
//		Y = rotationQuat * Y;
//		Z = rotationQuat * Z;
//	}
//
//	if (dy != 0)
//	{
//		float DeltaY = (float)dy * Sensitivity;
//
//		Quat rotationQuat = Quat::RotateAxisAngle(X, DeltaY);
//
//		Y = rotationQuat * Y;
//		Z = rotationQuat * Z;
//
//		if (Y.y < 0.0f)
//		{
//			Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
//			Y = Z.Cross(X);
//		}
//	}
//
//	Position = Reference + Z * Position.Length();
//}
//
//float3 ModuleCamera3D::RotateVector(const float3& u, float angle, const float3& v)
//{
//	Quat rotationQuat = Quat::RotateAxisAngle(v, angle);
//	return rotationQuat * u;
//}

// -----------------------------------------------------------------
//void ModuleCamera3D::CalculateViewMatrix()
//{
//	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -(X.Dot(Position)), -(Y.Dot(Position)), -(Z.Dot(Position)), 1.0f);
//}