#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Primitive.h"
#include "SDL\include\SDL_opengl.h"
#include "C_Camera.h"
#include "ModuleTextures.h"


#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

#ifdef _DEBUG
#pragma comment (lib, "MathGeoLib/libx86/Debug2/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#else
#pragma comment (lib, "MathGeoLib/libx86/Release2/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#endif // _DEBUG


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ProjectionMatrix.SetIdentity();
	mainGameCam = nullptr;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
static const GLfloat CubeVertices[] = {
	-1, -1, -1,
	1, -1, -1,
	1, 1, -1,
	-1, 1, -1,
	-1, -1, 1,
	1, -1, 1,
	1, 1, 1,
	-1, 1, 1
};
static const GLuint CubeIndices[] = {
	0, 1, 3, 3, 1, 2,
	1, 5, 2, 2, 5, 6,
	5, 4, 6, 6, 4, 7,
	4, 0, 7, 7, 0, 3,
	3, 2, 7, 7, 2, 6,
	4, 5, 0, 0, 5, 1
};
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (ret == true)
	{
		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);

		glewInit();
	}



	Grid.axis = true;


	return ret;
}

bool ModuleRenderer3D::Start()
{
	return false;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//Bind game camera framebuffer
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(App->camera->camera->GetProjetionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->camera->GetViewMatrix());

	glBindFramebuffer(GL_FRAMEBUFFER, App->camera->camera->frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// Finish game camera frame buffer binding

	lights[0].SetPos(0, 0, 0);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	/*App->editor->AddFPS(App->GetDT());*/


	return UPDATE_CONTINUE;
}

update_status ModuleRenderer3D::PostUpdate(float dt)
{
	Grid.Render();

	App->geoLoader->RenderScene();

	/*DrawLine(ls.a, ls.b);*/

	if (mainGameCam != nullptr)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//Bind game camera framebuffer
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(mainGameCam->GetProjetionMatrix());

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(mainGameCam->GetViewMatrix());

		glBindFramebuffer(GL_FRAMEBUFFER, mainGameCam->frameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// Finish game camera frame buffer binding

		App->geoLoader->RenderGameScene();

		/*DrawLine(ls.a, ls.b);*/
	}

	Grid.Render();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (App->editor->DrawEditor() == UPDATE_STOP)
	{
		return UPDATE_STOP;
	}

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	SDL_GL_DeleteContext(context);

	return true;
}

C_Camera* ModuleRenderer3D::GetMainCamera()
{
	return mainGameCam;
}

void ModuleRenderer3D::DrawBox(float3* corners, float3 color)
{
	glBegin(GL_LINES);
	glColor3fv(color.ptr());

	int indices[24] = { 0,2,2,6,6,4,4,0,0,1,1,3,3,2,4,5,6,7,5,7,3,7,1,5 };
	for (size_t i = 0; i < 24; i++)
		glVertex3fv(corners[indices[i]].ptr());

	// Reset color
	glColor3f(255.f, 255.f, 255.f);
	glEnd();
}

void ModuleRenderer3D::DrawLine(float3 a, float3 b)
{
	glBegin(GL_LINES);

	glColor3fv(float3(255.0f, 0, 255.0f).ptr());


	glVertex3fv(a.ptr());
	glVertex3fv(b.ptr());

	glColor3f(255.f, 255.f, 255.f);

	glEnd();
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glLoadMatrixf(App->camera->camera->GetProjetionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::SetMainCamera(C_Camera* cam)
{
	//No main camera
	if (cam == nullptr) {
		mainGameCam = nullptr;
		LOG("There's no game camera");
		return;
	}

	//Switch main cameras
	if (mainGameCam != nullptr)
		mainGameCam->isMainCamera = false;

	cam->isMainCamera = true;

	mainGameCam = cam;
}

