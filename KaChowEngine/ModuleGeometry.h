#pragma once

#ifndef MODULE_GEOMETRY
#define MODULE_GEOMETRY

#include "Module.h"
#include "Globals.h"
#include <vector>

class ModuleGeometry : public Module
{
public:

	ModuleGeometry(Application* app, bool start_enabled = true);
	~ModuleGeometry();

	bool Init() override;
	bool CleanUp() override;

private:

};

#endif