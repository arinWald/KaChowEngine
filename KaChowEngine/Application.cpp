#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	geoLoader = new ModuleGeometry(this);
	renderer3D = new ModuleRenderer3D(this);
	texture2D = new ModuleTextures(this);
	camera = new ModuleCamera3D(this);
	editor = new ModuleEditor(this);
	scene = new ModuleScene(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update() in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(texture2D);

	AddModule(geoLoader);

	AddModule(scene);
	// Renderer last!
	AddModule(renderer3D);
	AddModule(editor);
}

Application::~Application()
{
	for (std::vector<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it)
	{
		delete (*it);
		(*it) = nullptr;
	}
}

bool Application::Init()
{
	bool ret = true;

	maxFPS = 400;
	timeSpeed = 1;

	// Call Init() in all modules
	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret; ++it)
	{
		(*it)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret; ++it)
	{
		(*it)->Start();
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (editor->vSync)
	{
		lastMsFrame = ms_timer.Read();

		float timeToWait = 1000.0f / (float)maxFPS;

		float result = timeToWait - lastMsFrame;

		if (lastMsFrame < timeToWait)
			SDL_Delay(static_cast<Uint32>(fabs(result)));

		lastMsFrame = ms_timer.Read();

	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret == UPDATE_CONTINUE; ++it)
	{
		ret = (*it)->PreUpdate(dt);
	}

	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret == UPDATE_CONTINUE; ++it)
	{
		ret = (*it)->Update(dt);
	}

	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret == UPDATE_CONTINUE; ++it)
	{
		ret = (*it)->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	for (std::vector<Module*>::reverse_iterator it = list_modules.rbegin(); it != list_modules.rend() && ret; ++it)
	{
		ret = (*it)->CleanUp();
	}
	return ret;
}

float Application::GetDT()
{
	return dt;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::LOGToEditor(const char* tmp_string)
{
	editor->ConsoleLog(tmp_string);
}

float Application::GetGameDT()
{
	return gamedt;
}

void Application::SetGameDT()
{
	game_timer.Start();
	gamedt = ((float)game_timer.Read() / 1000.0f) * timeSpeed;
}

void Application::StopGameDT()
{
	game_timer.Stop();
	gamedt = 0;
}

void Application::PauseGameDT()
{
	if (gamedt == 0)
		gamedt = ((float)game_timer.Read() / 1000.0f) * timeSpeed;
	else
		gamedt = 0;
}