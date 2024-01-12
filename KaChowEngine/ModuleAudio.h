#pragma once

#include "Module.h"
#include "Globals.h"

// Wwise includes
#include <AK/SoundEngine/Common/AkMemoryMgr.h>                  // Memory Manager interface
#include <AK/SoundEngine/Common/AkModule.h>                     // Default memory manager
#include <AK/SoundEngine/Common/IAkStreamMgr.h>                 // Streaming Manager
#include <AK/Tools/Common/AkPlatformFuncs.h>                    // Thread defines
#include <AK/SoundEngine/Win32/AkFilePackageLowLevelIODeferred.h>// Sample low-level I/O implementation
#include <AK/SoundEngine/Common/AkSoundEngine.h>                // Sound engine
#include <AK/MusicEngine/Common/AkMusicEngine.h>                // Music Engine
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>              // Spatial Audio

class ModuleAudio : public Module
{
public:

	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Init();
	bool Start();
	bool InitSoundEngine();
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	bool CleanUp();

	void ProcessAudio();
	void TermSoundEngine();

	void RegisterGameObject(unsigned int id);
	void UnregisterGameObject(unsigned int id);

private:
	
	CAkFilePackageLowLevelIODeferred g_lowLevelIO;

};

