#include "Application.h"
#include "C_Camera.h"
#include "GameObject.h"
#include "C_Transform.h"

C_Camera::C_Camera() :Component(nullptr)
{
	typeCameraSelected = 0;

	FOV = 60;
	farDistance = 500.0f;
	nearDistance = 0.1f;

	type = ComponentType::CAMERA;
	mParent = nullptr;

	//Frustum
	frustum.type = PerspectiveFrustum;
	frustum.nearPlaneDistance = nearDistance;
	frustum.farPlaneDistance = farDistance; //inspector
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.verticalFov = FOV * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.7f);
	frustum.pos = float3(0, 0, 0);
}

C_Camera::~C_Camera()
{
	if (isMainCamera) App->renderer3D->SetMainCamera(nullptr);

	glDeleteFramebuffers(1, &cameraBuffer);
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteFramebuffers(1, &renderObjBuffer);
}

void C_Camera::OnEditor()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		if (ImGui::SliderInt("FOV", &FOV, 5, 200)) {
			frustum.verticalFov = FOV * DEGTORAD;
			frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.7f);
		}
		if (ImGui::Button("Reset")) {
			FOV = 60.0f;

			frustum.verticalFov = FOV * DEGTORAD;
			frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.7f);
		}

		ImGui::SliderFloat("Near Distance", &frustum.nearPlaneDistance, 0.1f, 500.f);
		if (ImGui::Button("Reset")) {
			frustum.nearPlaneDistance = 0.1f;
		}

		ImGui::SliderFloat("Far Distance", &frustum.farPlaneDistance, 500.f, 1000.f);
		if (ImGui::Button("Reset")) {
			frustum.farPlaneDistance = 500.f;
		}
	}
}

void C_Camera::Update()
{
	if (mParent == nullptr) return;

	frustum.pos = mParent->mTransform->getPosition();
	float4x4 m = mParent->mTransform->getGlobalMatrix();
	frustum.up = m.RotatePart().Col(1).Normalized();
	frustum.front = m.RotatePart().Col(2).Normalized();
}

void C_Camera::Look(const float3& Position, const float3& Reference)
{
	frustum.pos = Position;
	reference = Reference;

	LookAt(reference);
}

void C_Camera::LookAt(const float3& Spot)
{
	reference = Spot;
	frustum.front = (Spot - frustum.pos).Normalized();
	float3 X = float3(0, 1, 0).Cross(frustum.front).Normalized();
	frustum.up = frustum.front.Cross(X);
}

void C_Camera::Move(const float3& Movement)
{
	frustum.pos += Movement;
}

float* C_Camera::GetViewMatrix()
{
	viewMatrix = frustum.ViewMatrix();
	viewMatrix.Transpose();
	return viewMatrix.ptr();
}

float* C_Camera::GetProjetionMatrix()
{
	projectionMatrix = frustum.ProjectionMatrix();
	projectionMatrix.Transpose();
	return projectionMatrix.ptr();
}