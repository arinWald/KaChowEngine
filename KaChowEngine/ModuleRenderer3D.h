#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "glmath.h"


#include "Glew/include/glew.h"

#include "MathGeoLib/include/Math/float3x3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "Primitive.h"

//todo: REMOVE this before 1st delivery!!


#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	bool Start() override;
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void DrawBox(float3* corners, float3 color);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	
	//You won't need this after using Frustum
	mat4x4 ProjectionMatrix;
	CPlane Grid;

	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix;

	GLuint VBO;
	GLuint EBO;
	GLuint VAO;
};	
