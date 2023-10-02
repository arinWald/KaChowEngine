#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
//// Pilota
//#include "Glew/include/glew.h"

#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_sdl.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include "SDL/include/SDL_opengl.h"
#endif

#ifdef _DEBUG
#pragma comment (lib, "MathGeoLib/libx86/Debug2/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#else
#pragma comment (lib, "MathGeoLib/libx86/Release2/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#endif // _DEBUG

static const GLfloat CubeVertices[]
= {  0.500000, - 0.500000, - 0.500000,
 0.500000, - 0.500000, 0.500000,
 - 0.500000, - 0.500000, 0.500000,
 - 0.500000, - 0.500000, - 0.500000,
 0.500000, 0.500000, - 0.500000,
 0.500000, 0.500000, 0.500000,
 - 0.500000, 0.500000, 0.500000,
 - 0.500000, 0.500000, - 0.500000 };

static const GLuint Cubeindices[]
= { 2 , 1 , 1 , 3 , 2 , 1, 4 , 3 , 1,
 5 , 3 , 2 ,8 , 4 , 2, 7 , 1 , 2,
 5 , 3 , 3 ,6 , 4 , 3 ,2 , 1 , 3,
 2 , 2 , 4 ,6 , 3 , 4 ,7 , 4 , 4,
 7 , 3 , 5 ,8 , 4 , 5 ,4 , 1 , 5,
 5 , 4 , 6 ,1 , 1 , 6 ,4 , 2 , 6,
 1 , 4 , 1 ,2 , 1 , 1 ,4 , 3 , 1,
 6 , 2 , 2 ,5 , 3 , 2 ,7 , 1 , 2,
 1 , 2 , 7 ,5 , 3 , 7 ,2 , 1 , 7,
 3 , 1 , 4 ,2 , 2 , 4 ,7 , 4 , 4,
 3 , 2 , 5 ,7 , 3 , 5 ,4 , 1 , 5,
 8 , 3 , 6 ,5 , 4 , 6 ,4 , 2 , 6 };

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	//GLEW
	GLenum err = glewInit();

	if (GLEW_OK != err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		LOG("Error: %a\n", glewGetErrorString(err));
	}
	else {
		LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	}

	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);	
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error

		//!!!! Add GLenum before "error" if something happen "pilota"
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		// Pilota
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		
		// Pilota
		glEnable(GL_TEXTURE_2D);

		hola = CCylinder(3, 5);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	Grid.axis = true;

	//VBO = 0;
	//// Assigna id a buffer
	//glGenBuffers(1, &VBO);
	//// A quin buffer volem treballar
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//// 
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	//// Al acabar, reset id del buffer
	//glBindBuffer(GL_ARRAY_BUFFER, 0);


	VBO = 0;
	EBO = 0;
	VAO = 0;

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Cubeindices), Cubeindices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);


	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Pilota
	//Color c = cam->background;
	//glClearColor(c.r, c.g, c.b, c.a);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	// "pilota"
	/*App->editor->AddFPS(App->GetDT());*/

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	//GLUquadric* qobj = gluNewQuadric();  // Crear un objeto cu�drico
	//gluSphere(qobj, 5, 10, 10);
	
	//hola.Render();

	// Pilota
	//App->level->Draw();

	/*if (debug_draw == true)
	{
		BeginDebugDraw();
		App->DebugDraw();
		EndDebugDraw();
	}*/

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Pilota aquesta linia s'ha de treure un cop els buffers  de update estiguin ben fets
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// � bind and use other buffers
	glDrawArrays(GL_TRIANGLES, 0, (sizeof(CubeVertices) / sizeof(float)) / 3);
	glDisableClientState(GL_VERTEX_ARRAY);

	Grid.Render();

	App->editor->DrawEditor();

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//todo: USE MATHGEOLIB here BEFORE 1st delivery! (TIP: Use MathGeoLib/Geometry/Frustum.h, view and projection matrices are managed internally.)
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(ProjectionMatrix.M);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
