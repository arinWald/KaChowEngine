#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
	name = "Window";
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		// New OpenGL Stuff

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		e_width = width;
		e_height = height;

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetFullscreen()
{
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
}

void ModuleWindow::SetResizable()
{
	SDL_SetWindowFullscreen(window, SDL_WINDOW_BORDERLESS);
}

void ModuleWindow::ChangeWidth()
{
	SDL_SetWindowSize(window, e_width, e_height);
	App->renderer3D->OnResize(e_width, e_height);
}

void ModuleWindow::ChangeHeight()
{
	SDL_SetWindowSize(window, e_width, e_height);
	App->renderer3D->OnResize(e_width, e_height);
}

bool ModuleWindow::SaveConfig(JsonParser& node) const
{
	node.SetNewJsonNumber(node.ValueToObject(node.GetRootValue()), "width", e_width);
	node.SetNewJsonNumber(node.ValueToObject(node.GetRootValue()), "height", e_height);
	node.SetNewJsonNumber(node.ValueToObject(node.GetRootValue()), "brightness", App->editor->bright);

	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "fullscreen", App->editor->fullscreen);

	return true;
}

bool ModuleWindow::LoadConfig(JsonParser& node)
{
	e_width = (int)node.JsonValToNumber("width") * SCREEN_SIZE;
	e_height = (int)node.JsonValToNumber("height") * SCREEN_SIZE;
	App->editor->bright = (float)node.JsonValToNumber("brightness");

	App->editor->fullscreen = node.JsonValToBool("fullscreen");

	/*if (App->editor->fullscreen) SetFullscreen();
	else
	{
		SetResizable();
	}*/
	SetSize(e_width, e_height);

	return true;
}

void ModuleWindow::SetSize(int width, int height)
{
	this->e_height = height;
	this->e_width = width;

	SDL_SetWindowSize(window, width, height);
}