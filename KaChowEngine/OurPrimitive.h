#pragma once

class Mesh;
class GameObject;

enum class ShapeType {
	EMPTY,
	PLANE,
	CUBE,
	SPHERE,
	CYLINDER,
	MONKEY,
	ICOSHPERE,
	CONE,
	TORUS,
};

class OurPrimitive
{
public:
	static GameObject* CreatePrimitive(ShapeType type);

private:

};

