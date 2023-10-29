#include "OurPrimitive.h"
#include "Application.h"
#include "GameObject.h"
#include "C_Mesh.h"

GameObject* OurPrimitive::CreatePrimitive(ShapeType type)
{
	// Empty
	GameObject* gameObject = new GameObject();
	if (type == ShapeType::EMPTY)
	{
		gameObject->name = "Empty Object";
		return gameObject;
	}

	//// Not Empty
	//switch (type)
	//{
	//case ShapeType::PLANE:
	//	gameObject = App->geoLoader->LoadFile("Assets/Models/plane.fbx");

	//	gameObject->name = "Plane";
	//	return gameObject;
	//	break;
	//case ShapeType::CUBE:
	//	gameObject = App->geoLoader->LoadFile("Assets/Models/cube.fbx");

	//	gameObject->name = "Cube";
	//	return gameObject;
	//	break;
	//case ShapeType::SPHERE:
	//	break;
	//case ShapeType::CYLINDER:
	//	break;
	//case ShapeType::MONKEY:
	//	break;
	//case ShapeType::ICOSHPERE:
	//	break;
	//case ShapeType::CONE:
	//	break;
	//case ShapeType::TORUS:
	//	break;
	//default:
	//	break;
	//}
}