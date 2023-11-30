#include "Application.h"
#include "C_Camera.h"
#include "GameObject.h"


C_Camera::C_Camera() : Component(nullptr)
{
	type = ComponentType::CAMERA;
	mParent = nullptr;
	SetCam();
	GenBuffer();
}

C_Camera::C_Camera(GameObject* owner) : Component(owner)
{
	type = ComponentType::CAMERA;
	this->mParent = owner;
	SetCam();
	GenBuffer();
}

C_Camera::~C_Camera()
{
	glDeleteFramebuffers(1, &cameraBuffer);
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteFramebuffers(1, &renderObjBuffer);
}


void C_Camera::SetCam()
{
	FrustumCam.type = FrustumType::PerspectiveFrustum;
	FrustumCam.nearPlaneDistance = 0.1f;
	FrustumCam.farPlaneDistance = 500.f;
	FrustumCam.front = float3::unitZ;
	FrustumCam.up = float3::unitY;

	FrustumCam.verticalFov = 60.0f * DEGTORAD;
	FrustumCam.horizontalFov = 2.0f * atanf(tanf(FrustumCam.verticalFov / 2.0f) * 1.7f);

	FrustumCam.pos = float3(0, 0, 0);
}

void C_Camera::LookAt(const float3& target)
{
	FrustumCam.front = (target - FrustumCam.pos).Normalized();
	float3 X = float3(0, 1, 0).Cross(FrustumCam.front).Normalized();
	FrustumCam.up = FrustumCam.front.Cross(X);
}

void C_Camera::GenBuffer()
{
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &cameraBuffer);
	glBindTexture(GL_TEXTURE_2D, cameraBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	float color[4] = { 0.1,0.1,0.1,0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cameraBuffer, 0);

	glGenRenderbuffers(1, &renderObjBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderObjBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderObjBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

float* C_Camera::GetViewMatrix()
{

	viewMatrix = FrustumCam.ViewMatrix();

	viewMatrix.Transpose();

	return viewMatrix.ptr();
}

float* C_Camera::GetProjectionMatrix()
{

	projectionMatrix = FrustumCam.ProjectionMatrix();

	projectionMatrix.Transpose();

	return projectionMatrix.ptr();
}

void C_Camera::OnEditor()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		if (ImGui::SliderInt("FOV", &FOV, 5, 200)) {
			FrustumCam.verticalFov = FOV * DEGTORAD;
			FrustumCam.horizontalFov = 2.0f * atanf(tanf(FrustumCam.verticalFov / 2.0f) * 1.7f);
		}
		if (ImGui::Button("Reset")) {
			FOV = 60.0f;

			FrustumCam.verticalFov = FOV * DEGTORAD;
			FrustumCam.horizontalFov = 2.0f * atanf(tanf(FrustumCam.verticalFov / 2.0f) * 1.7f);
		}

		ImGui::SliderFloat("Near Distance", &FrustumCam.nearPlaneDistance, 0.1f, 500.f);
		if (ImGui::Button("Reset")) {
			FrustumCam.nearPlaneDistance = 0.1f;
		}

		ImGui::SliderFloat("Far Distance", &FrustumCam.farPlaneDistance, 500.f, 1000.f);
		if (ImGui::Button("Reset")) {
			FrustumCam.farPlaneDistance = 500.f;
		}
	}
}

void C_Camera::CameraTransform()
{
	if (mParent == nullptr) return;
	FrustumCam.pos = mParent->mTransform->mPosition;
	float4x4 matrix = mParent->mTransform->GetTransformMatrix();

	FrustumCam.up = matrix.RotatePart().Col(1).Normalized();
	FrustumCam.front = matrix.RotatePart().Col(2).Normalized();
}