#include "C_Transform.h"

C_Transform::C_Transform() : Component(nullptr)
{

}

C_Transform::C_Transform(GameObject* parent) : Component(parent)
{
	type = ComponentType::TRANSFORM;
	this->mParent = parent;

	//mPosition = { 0, 0, 0 };
	//mScale = { 0, 0, 0 };
	//mRotation = { 0, 0, 0};

	resetMatrix();
}

C_Transform::~C_Transform()
{
}

void C_Transform::OnEditor()
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::Text("X\t\t Y\t\t Z");
		ImGui::InputFloat3("Position", mPosition.ptr());

		ImGui::Text("X\t\t Y\t\t Z");
		ImGui::InputFloat3("Rotation", mRotation.ptr());

		ImGui::Text("X\t\t Y\t\t Z");
		ImGui::InputFloat3("Scale", mScale.ptr());
	}


	calculateMatrix();
}

void C_Transform::Enable()
{
	active = false;
}

void C_Transform::Disable()
{
	active = false;
}

float4x4 C_Transform::getGlobalMatrix()
{
	// fer getter de parent
	if (mParent == nullptr) return getLocalMatrix();
	return mLocalMatrix * mParent->mTransform->getGlobalMatrix();
}

float4x4 C_Transform::getLocalMatrix()
{
	return mLocalMatrix;
}

void C_Transform::resetMatrix()
{
	mLocalMatrix = { 1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1 };
	mPosition = { 0,0,0 };
	mRotation = { 0,0,0 };
	mScale = { 0,0,0 };
}

float3 C_Transform::getPosition(bool globalPosition)
{
	if (!globalPosition) return float3(mPosition);

	float4x4 m = getGlobalMatrix();
	return m.Row3(3);
}

void C_Transform::setPosition(float3 pos)
{
	mPosition = pos;
	calculateMatrix();
}

float3 C_Transform::getRotation()
{
	return mRotation;
}

void C_Transform::setRotation(float3 rotation)
{
	mRotation = rotation;
	calculateMatrix();
}

float3 C_Transform::getScale()
{
	return mScale;
}

void C_Transform::setScale(float3 scale)
{
	mScale = scale;
	calculateMatrix();
}



//void C_Transform::SetTransfoMatrix(float3 position, Quat rotation, float3 scale)
//{
//	// Pilota
//	mPosition = position;
//	mRotation = rotation;
//	mScale = scale;
//}

void C_Transform::calculateMatrix()
{
	float rx = mRotation.x * DEGTORAD;
	float ry = mRotation.y * DEGTORAD;
	float rz = mRotation.z * DEGTORAD;

	Quat q;
	q = Quat::FromEulerXYZ(rx, ry, rz);
	mLocalMatrix = float4x4::FromTRS(mPosition, q, mScale);
}
