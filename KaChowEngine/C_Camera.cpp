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

	CreateFrameBuffer();
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
	const char* listType[]{ "Perspective", "Orthographic" };

	if (ImGui::CollapsingHeader("Camera"))
	{
		//is main camera??
		if (isMainCamera)
		{
			ImGui::SameLine();
			ImGui::Text(" ( Main Camera )");
		}

		ImGui::Text("");

		//Chose type of camera
		ImGui::Text("Camera type:\t ");
		ImGui::SameLine();
		if (ImGui::Combo("##CameraType", &typeCameraSelected, listType, IM_ARRAYSIZE(listType)))
		{
			if (typeCameraSelected == 0)
				frustum.type = PerspectiveFrustum;

			if (typeCameraSelected == 1)
				frustum.type = OrthographicFrustum;
		}

		ImGui::Text("");

		//Fov camera
		ImGui::Text("FOV\t\t\t  ");
		ImGui::SameLine();
		if (ImGui::SliderInt("FOV", &FOV, 10, 170))
		{
			frustum.verticalFov = FOV * DEGTORAD;
			frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.7f);
		}

		ImGui::Text("");

		//Slider Set Near Distane
		ImGui::Text("Near Distance\t");
		ImGui::SameLine();
		if (ImGui::SliderFloat("##nearDistance", &nearDistance, 0.1f, farDistance))
		{
			frustum.nearPlaneDistance = nearDistance;
		}

		ImGui::Text("");

		//Input float Set Far Distane
		ImGui::Text("Far Distance\t ");
		ImGui::SameLine();
		if (ImGui::InputFloat("##farDistance", &farDistance))
		{
			if (farDistance <= nearDistance)
			{
				nearDistance = farDistance;
			}

			frustum.farPlaneDistance = farDistance;
		}

		ImGui::Text("");

		//Button Set main camera
		ImGui::Text("");
		ImGui::SameLine((ImGui::GetWindowWidth() / 2) - 75);
		if (ImGui::Button("Set as main camera", ImVec2(150, 25)))
		{
			App->renderer3D->SetMainCamera(this);
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

void C_Camera::CreateFrameBuffer()
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
		LOG("Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	App->renderer3D->SetMainCamera(this);
}