#include "C_Transform.h"

C_Transform::C_Transform() : Component(nullptr)
{
	type = ComponentType::TRANSFORM;

	resetMatrix();
}

C_Transform::C_Transform(GameObject* parent) : Component(parent)
{
	type = ComponentType::TRANSFORM;
	this->mParent = parent;

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
		ImGui::DragFloat3("Position", mPosition.ptr(), 0.25f);

		ImGui::Text("X\t\t Y\t\t Z");
		ImGui::DragFloat3("Rotation", mRotation.ptr(), 0.25f);

		ImGui::Text("X\t\t Y\t\t Z");
		ImGui::DragFloat3("Scale", mScale.ptr(), 0.25f);
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
	mScale = { 1,1,1 };
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

void C_Transform::SetTransfoMatrix(float3 position, Quat rotation, float3 scale)
{
	float x = rotation.x * DEGTORAD;
	float y = rotation.y * DEGTORAD;
	float z = rotation.z * DEGTORAD;

	mLocalMatrix[0][0] = cos(y) * cos(z);
	mLocalMatrix[1][0] = -cos(x) * sin(z) + sin(y) * cos(z) * sin(x);
	mLocalMatrix[2][0] = sin(x) * sin(z) + sin(y) * cos(z) * cos(x);
	mLocalMatrix[3][0] = rotation.x;

	mLocalMatrix[0][1] = cos(y) * sin(z);
	mLocalMatrix[1][1] = cos(x) * cos(z) + sin(y) * sin(z) * sin(z);
	mLocalMatrix[2][1] = -sin(x) * cos(z) + sin(y) * sin(z) * cos(x);
	mLocalMatrix[3][1] = position.y;

	mLocalMatrix[0][2] = -sin(y);
	mLocalMatrix[1][2] = cos(y) * sin(x);
	mLocalMatrix[2][2] = cos(x) * cos(y);
	mLocalMatrix[3][2] = position.z;

	mLocalMatrix[0][3] = 0;
	mLocalMatrix[1][3] = 0;
	mLocalMatrix[2][3] = 0;
	mLocalMatrix[3][3] = 1;

	mLocalMatrix[0][0] *= scale.x;
	mLocalMatrix[1][1] *= scale.y;
	mLocalMatrix[2][2] *= scale.z;


	if (mParent->mParent->mTransform != nullptr)
	{
		mGlobalMatrixT = mParent->mParent->mTransform->mGlobalMatrixT * mLocalMatrix;
		//glTransformT = glTransformT.Transposed();
	}
	else {
		//glTransformT = lTransform.Transposed();
	}
}

void C_Transform::calculateMatrix()
{
	float rx = mRotation.x * DEGTORAD;
	float ry = mRotation.y * DEGTORAD;
	float rz = mRotation.z * DEGTORAD;

	Quat q;
	q = Quat::FromEulerXYZ(rx, ry, rz);
	mLocalMatrix = float4x4::FromTRS(mPosition, q, mScale).Transposed();
}
