#include "OurPrimitive.h"
#include "Application.h"
#include "GameObject.h"
#include "C_Mesh.h"

GameObject* OurPrimitive::CreatePrimitive(ShapeType type)
{
	// Empty
	if (type == ShapeType::EMPTY)
	{
        GameObject* emptyGameObject = new GameObject(App->scene->rootGameObject);
        emptyGameObject->name = "Empty Object";
        emptyGameObject->type = ShapeType::EMPTY;
		return emptyGameObject;
	}
    else
    {
        // Not Empty
        GameObject* gameObject = new GameObject();
        switch (type)
        {
        case ShapeType::PLANE:
            gameObject = App->geoLoader->LoadFile("Assets/Models/plane.fbx");
            gameObject->name = "Plane";
            gameObject->type = ShapeType::PLANE;
            return gameObject;
            break;

        case ShapeType::CUBE:
            gameObject = App->geoLoader->LoadFile("Assets/Models/cube.fbx");
            gameObject->name = "Cube";
            gameObject->type = ShapeType::CUBE;
            return gameObject;
            break;

        case ShapeType::SPHERE:
            gameObject = App->geoLoader->LoadFile("Assets/Models/sphere.fbx");
            gameObject->name = "Sphere";
            gameObject->type = ShapeType::SPHERE;
            return gameObject;
            break;

        case ShapeType::CYLINDER:
            gameObject = App->geoLoader->LoadFile("Assets/Models/cylinder.fbx");
            gameObject->name = "Cylinder";
            gameObject->type = ShapeType::CYLINDER;
            return gameObject;
            break;

        case ShapeType::MONKEY:
            gameObject = App->geoLoader->LoadFile("Assets/Models/monkey.fbx");
            gameObject->name = "Monkey";
            gameObject->type = ShapeType::MONKEY;
            return gameObject;
            break;

        case ShapeType::ICOSHPERE:
            gameObject = App->geoLoader->LoadFile("Assets/Models/icosphere.fbx");
            gameObject->name = "Icosphere";
            gameObject->type = ShapeType::ICOSHPERE;
            return gameObject;
            break;

        case ShapeType::CONE:
            gameObject = App->geoLoader->LoadFile("Assets/Models/cone.fbx");
            gameObject->name = "Cone";
            gameObject->type = ShapeType::CONE;
            return gameObject;
            break;

        case ShapeType::TORUS:
            gameObject = App->geoLoader->LoadFile("Assets/Models/torus.fbx");
            gameObject->name = "Torus";
            gameObject->type = ShapeType::TORUS;
            return gameObject;
            break;

        default:
            // Handle the case when an unsupported shape type is provided.
            // You can throw an exception, log an error, or return a default object.
            // Here, we'll return nullptr to indicate an error.
            return nullptr;
            break;
        }
    }
}