#include "Application.h"
#include "C_Camera.h"
#include "GameObject.h"
#include "C_Transform.h"

C_Camera::C_Camera() :Component(nullptr)
{

	type = ComponentType::CAMERA;
	mParent = nullptr;

	frustum.type = PerspectiveFrustum;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 500.f;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.verticalFov = 60.0f * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.7f); // 16:9 ~= 1,77777...
	frustum.pos = float3(0, 0, 0);
}

C_Camera::~C_Camera()
{
}

void C_Camera::PrintInspector()
{

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