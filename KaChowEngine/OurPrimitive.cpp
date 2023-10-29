#include "OurPrimitive.h"
#include "Application.h"
#include "GameObject.h"
#include "C_Mesh.h"

GameObject* OurPrimitive::CreatePrimitive(ShapeType type)
{
	GameObject* gameObject = new GameObject();
	// Empty
	if (type == ShapeType::EMPTY)
	{
		gameObject->name = "Empty Object";
		return gameObject;
	}

	// Not Empty
	switch (type)
	{
	case ShapeType::PLANE:
		gameObject = App->geoLoader->LoadFile("Assets/Models/plane.fbx");

		gameObject->name = "Plane";
		return gameObject;
		break;
	case ShapeType::CUBE:
		gameObject = App->geoLoader->LoadFile("Assets/Models/cube.fbx");

		gameObject->name = "Cube";
		return gameObject;

		break;
    case ShapeType::SPHERE:
        gameObject = App->geoLoader->LoadFile("Assets/Models/sphere.fbx");
        gameObject->name = "Sphere";
        return gameObject;

    case ShapeType::CYLINDER:
        gameObject = App->geoLoader->LoadFile("Assets/Models/cylinder.fbx");
        gameObject->name = "Cylinder";
        return gameObject;

    case ShapeType::MONKEY:
        gameObject = App->geoLoader->LoadFile("Assets/Models/monkey.fbx");
        gameObject->name = "Monkey";
        return gameObject;

    case ShapeType::ICOSHPERE:
        gameObject = App->geoLoader->LoadFile("Assets/Models/icosphere.fbx");
        gameObject->name = "Icosphere";
        return gameObject;

    case ShapeType::CONE:
        gameObject = App->geoLoader->LoadFile("Assets/Models/cone.fbx");
        gameObject->name = "Cone";
        return gameObject;

    case ShapeType::TORUS:
        gameObject = App->geoLoader->LoadFile("Assets/Models/torus.fbx");
        gameObject->name = "Torus";
        return gameObject;

    default:
        // Handle the case when an unsupported shape type is provided.
        // You can throw an exception, log an error, or return a default object.
        // Here, we'll return nullptr to indicate an error.
        return nullptr;

	}
}