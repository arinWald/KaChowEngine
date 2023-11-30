#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "MathGeoLib/include/Math/float3.h"
#include "C_Transform.h"
#include "C_Camera.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	sceneCam = new C_Camera();
	sceneCam->FrustumCam.pos = float3(0, 2, -10);

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	delete sceneCam;

	return true;
}

void ModuleCamera3D::Rotation()
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	Quat dir;
	sceneCam->FrustumCam.WorldMatrix().Decompose(float3(), dir, float3());

	if (dy != 0) {
		float DeltaY = (float)dy * mouseSens;

		Quat Y = Quat::identity;
		Y.SetFromAxisAngle(float3(1.0f, 0.0f, 0.0f), DeltaY * DEGTORAD);

		dir = dir * Y;
	}

	if (dx != 0) {
		float DeltaX = (float)dx * mouseSens;

		Quat X = Quat::identity;
		X.SetFromAxisAngle(float3(0.0f, 1.0f, 0.0f), DeltaX * DEGTORAD);

		dir = X * dir;
	}

	float4x4 matrix = sceneCam->FrustumCam.WorldMatrix();
	matrix.SetRotatePart(dir.Normalized());
	sceneCam->FrustumCam.SetWorldMatrix(matrix.Float3x4Part());
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	int wheel = -App->input->GetMouseZ();
	float speed = 3.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) sceneCam->FrustumCam.pos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) sceneCam->FrustumCam.pos.y -= speed;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) sceneCam->FrustumCam.pos += sceneCam->FrustumCam.front * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) sceneCam->FrustumCam.pos -= sceneCam->FrustumCam.front * speed;


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) sceneCam->FrustumCam.pos -= sceneCam->FrustumCam.WorldRight() * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) sceneCam->FrustumCam.pos += sceneCam->FrustumCam.WorldRight() * speed;

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) Rotation();

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		if (App->scene->selectedGameObj != nullptr) {
			float3 target = App->scene->selectedGameObj->mTransform->mPosition;
			sceneCam->LookAt(target);

			float TargetDist = sceneCam->FrustumCam.pos.Distance(target);

			Rotation();

			sceneCam->FrustumCam.pos = target + (sceneCam->FrustumCam.front * -TargetDist);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		if (App->scene->selectedGameObj != nullptr) {
			float3 target = App->scene->selectedGameObj->mTransform->mPosition;

			sceneCam->LookAt(target);
		}
	}

	if (wheel != 0) sceneCam->FrustumCam.pos += sceneCam->FrustumCam.front * speed * -wheel;



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
//float* ModuleCamera3D::GetViewMatrix()
//{
//
//	viewMatrix = FrustumCam.ViewMatrix();
//
//	viewMatrix.Transpose();
//
//	return viewMatrix.ptr();
//}
//
//float* ModuleCamera3D::GetProjectionMatrix()
//{
//
//	projectionMatrix = sceneCam->FrustumCam.ProjectionMatrix();
//
//	projectionMatrix.Transpose();
//
//	return projectionMatrix.ptr();
//}


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