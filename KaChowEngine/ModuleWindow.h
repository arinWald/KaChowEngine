#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "JsonParser.h"
#include "Parser/parson.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);
	void SetFullscreen();
	void SetResizable();
	void ChangeWidth();
	void ChangeHeight();

	bool SaveConfig(JsonParser& node) const;
	bool LoadConfig(JsonParser& node);

	void SetSize(int width, int height);

public:
	//The window we'll be rendering to
	SDL_Window* window;
	int e_width, e_height;

	//The surface contained by the window
	SDL_Surface* screen_surface;
};

#endif // __ModuleWindow_H__