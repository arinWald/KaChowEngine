#pragma once

#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Module.h"
#include "Globals.h"
#include <vector>
#include <string>
#include "ImGui/imgui.h"

#include "Glew/include/glew.h"


class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init() override;
	update_status DrawEditor();
	bool CleanUp() override;

	void AddHistogramData(const float aFPS, std::vector<float>& aMS);

	void ConsoleLog(const char* tmp_string);

	void GetHardwareInfo();

	std::vector<float> mFPSLog;
	std::vector<float> mMsLog;

private:
	int cvCounter;

	// Config window 
	bool fullscreen;
	float screenWidth;

	// Config render opengl
	bool lightning;
	bool isCullFace;
	bool depthTest;
	bool colorMat;
	bool texture2D;
	
	bool isWireframe;
	float bright;
	float bright_aux;

	bool isActivatedInspector;
	bool isActivatedHierarchy;
	bool isActivatedConsole;
	bool isActivatedDemo;

	// Hardware info
	std::string caps;
	std::string SDLVersion;

	uint CPUCount;
	uint CPUCache;
	float systemRAM;

	uint GPUVendor = 0;
	uint GPUDevice = 0;
	char GPUBrand[250] = "";
	float VRAMBudget = 0.f;
	float VRAMUsage = 0.f;
	float VRAMAvailable = 0.f;
	float VRAMReserved = 0.f;

public:
	ImGuiIO* io;

	std::vector<std::string>* logVector;
	bool vSync;
};

#endif