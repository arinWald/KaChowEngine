#pragma once

#ifndef MODULE_GEOMETRY
#define MODULE_GEOMETRY

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include <vector>

#include "Glew/include/glew.h"

#include "GameObject.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Glew/libx86/glew32.lib")


#define VERTEX_ARGUMENTS 5

struct Mesh
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;
	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

	GLuint id_texture;

	GLuint texture_width = 0, texture_height = 0;

	GameObject* owner;

	/*uint VBO;
	uint EBO;*/


	void Render();
	vec3 GetVectorFromIndex(float* startValue);
	void RenderMeshDebug(/*bool* vertexNormals, */bool* faceNormals);
};


class ModuleGeometry : public Module
{
public:

	ModuleGeometry(Application* app, bool start_enabled = true);
	~ModuleGeometry();

	bool Init() override;
	bool Start() override;
	bool CleanUp() override;
	void DestroyMesh(Mesh* mesh);
	GameObject* LoadFile(const char* file_path);
	void ImportMesh(aiMesh* aiMesh, GameObject* gameObject);
	void BufferMesh(Mesh* mesh);
	void RenderScene();

private:
	GameObject* newGameObject;
public:
	
	std::vector<Mesh*> meshes;
	const char* file_path;
};

#endif