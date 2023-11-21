#include "C_Camera.h"
#include "ModuleGeometry.h"
#include "Application.h"
#include "C_Transform.h"
#include "Glew/include/glew.h"

C_Camera::C_Camera() : Component(nullptr)
{
	type = ComponentType::CAMERA;
	mParent = nullptr;
	SetCam();
	GenFrameBuffer();
}

C_Camera::C_Camera(GameObject* parent) : Component(parent)
{
	type = ComponentType::CAMERA;
	this->mParent = parent;
	SetCam();
	GenFrameBuffer();
}

C_Camera::~C_Camera()
{
	glDeleteFramebuffers(1, &camBuffer);
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteFramebuffers(1, &renderObjBuffer);
	
}

void C_Camera::OnEditor()
{

	if (ImGui::CollapsingHeader("Camera"))
	{

	}
}

void C_Camera::SetCam()
{
	frustrumCamera.type = FrustumType::PerspectiveFrustum;
	frustrumCamera.nearPlaneDistance = 0.1f;
	frustrumCamera.farPlaneDistance = 500.f;
	frustrumCamera.front = float3::unitZ;
	frustrumCamera.up = float3::unitY;
	
	frustrumCamera.verticalFov = fov * DEGTORAD;
	frustrumCamera.horizontalFov = 2.0f * atanf(tanf(frustrumCamera.verticalFov / 2.0f) * 1.7f);

	frustrumCamera.pos = float3(0, 0, 0);
}

void C_Camera::GenFrameBuffer()
{
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &camBuffer);
	glBindTexture(GL_TEXTURE_2D, camBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	float color[4] = { 0.1,0.1,0.1,0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, camBuffer, 0);

	glGenRenderbuffers(1, &renderObjBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderObjBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderObjBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG("Framebuffer error: not complete ");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

float* C_Camera::GetViewMatrix()
{
	viewMatrix = frustrumCamera.ViewMatrix();
	viewMatrix.Transpose();

	return viewMatrix.ptr();
}

float* C_Camera::GetProjectMatrix()
{
	projectionMatrix = frustrumCamera.ProjectionMatrix();
	projectionMatrix.Transpose();

	return projectionMatrix.ptr();
}

void C_Camera::LookAt(const float3& target)
{
	frustrumCamera.front = (target - frustrumCamera.pos).Normalized();
	float3 X = float3(0, 1, 0).Cross(frustrumCamera.front).Normalized();
	frustrumCamera.up = frustrumCamera.front.Cross(X);
}

void C_Camera::TransformCam()
{
	if (mParent == nullptr) return;

	frustrumCamera.pos = mParent->mTransform->mPosition;
	float4x4 matrix = mParent->mTransform->getLocalMatrix();

	// Get Y axis UP
	frustrumCamera.up = matrix.RotatePart().Col(1).Normalized();
	// Get Z axis FRONT
	frustrumCamera.front = matrix.RotatePart().Col(2).Normalized();
}

bool C_Camera::HasBoundingBoxIsnide(Mesh* refBox)
{
	// Array to store the corner points of the bounding box
	float3 corners[8];
	// Variable to track how many corners of the bounding box are inside the frustum
	int howMany = 0;

	// Retrieve the corner points of the bounding box
	refBox->AABB_box.GetCornerPoints(corners);

	// Array to store the six planes of the camera frustum
	Plane m_plane[6];
	// Retrieve the planes of the camera frustum
	frustrumCamera.GetPlanes(m_plane);

	// Iterate through each plane of the frustum
	for (int p = 0; p < 6; ++p) {
		// Initialize variables for counting points inside the frustum
		int iInCount = 8;
		int iPtIn = 1;

		// Check each corner of the bounding box against the current frustum plane
		for (int i = 0; i < 8; ++i) {
			// Check if the corner is on the positive side of the plane
			if (m_plane[p].IsOnPositiveSide(corners[i])) {
				// Corner is outside the frustum
				iPtIn = 0;
				--iInCount;
			}
		}

		// If all corners are outside the frustum, return false
		if (iInCount == 0)
			return false;

		// Update the count of corners inside the frustum
		howMany += iPtIn;
	}

	// If all corners are inside the frustum, return true
	if (howMany == 6)
		return true;

	// Bounding box is partially inside the frustum
	return false;
}


void C_Camera::Enable()
{
	active = false;
}

void C_Camera::Disable()
{
	active = false;
}