#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleGeometry.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"
#include "JsonParser.h"
#include "ModuleAudio.h"

#include<vector>

enum class GameState
{
	PLAY,
	PAUSE,
	STOP
};

class Modules;
class ModuleWindow;
class ModuleInput;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleEditor;
class ModuleGeometry;
class ModuleTextures;
class ModuleScene;
class ModuleAudio;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleGeometry* geoLoader;
	ModuleRenderer3D* renderer3D;
	ModuleTextures* texture2D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	ModuleScene* scene;
	ModuleAudio* audio;

private:

	Timer	ms_timer;
	Timer game_timer;
	float	dt;
	float gamedt;
	uint    maxFPS;
	float    lastMsFrame;
	std::vector<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	float GetDT();
	void SetDT(float dt);
	float DTGame();
	bool IsRunning();
	bool IsPaused();
	GameState GetState();
	void SetState(GameState gameState);
	void LOGToEditor(const char* tmp_string);

	float GetGameDT();
	void SetGameDT();
	void StopGameDT();
	void PauseGameDT();

	float	dtGame;
	GameState gameState = GameState::STOP;

	JsonParser jsonFile;

	bool saveRequested;
	bool loadRequested;

	void SaveConfigRequest() { saveRequested = true; };
	void LoadConfigRequest() { loadRequested = true; }

	void SaveConfig();
	void LoadConfig();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;