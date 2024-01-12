#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

#include "GameObject.h"

#include <string>

#include <iostream>
#include <filesystem>


ModuleAudio::ModuleAudio(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleAudio::~ModuleAudio()
{
}

bool ModuleAudio::Init()
{
	bool ret = true;


	return ret;
}

bool ModuleAudio::Start()
{

	return true;
}

bool ModuleAudio::InitSoundEngine()
{

    AkMemSettings memSettings;
    AK::MemoryMgr::GetDefaultSettings(memSettings);

    if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
    {
        LOG("Couldn't create the Memory Manager");
        return false;
    }
    //

    // Create and initialize an instance of the default streaming manager. Note

    // that you can override the default streaming manager with your own. 

    //


    AkStreamMgrSettings stmSettings;
    AK::StreamMgr::GetDefaultSettings(stmSettings);

    // Customize the Stream Manager settings here.


    if (!AK::StreamMgr::Create(stmSettings))
    {
        assert(!"Could not create the Streaming Manager");
        return false;
    }

    //

    // Create a streaming device.

    // Note that you can override the default low-level I/O module with your own. 

    //

    AkDeviceSettings deviceSettings;
    AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

    // Customize the streaming device settings here.


    // CAkFilePackageLowLevelIODeferred::Init() creates a streaming device

    // in the Stream Manager, and registers itself as the File Location Resolver.

    if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
    {
        assert(!"Could not create the streaming device and Low-Level I/O system");
        return false;
    }

    //

    // Create the Sound Engine

    // Using default initialization parameters

    //


    AkInitSettings          initSettings;
    AkPlatformInitSettings  platformInitSettings;

    AK::SoundEngine::GetDefaultInitSettings(initSettings);
    AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

    initSettings.uMaxNumPaths = 16;

    if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
    {
        return false;
    }

    //

    // Initialize the music engine

    // Using default initialization parameters

    //


    AkMusicSettings musicInit;
    AK::MusicEngine::GetDefaultInitSettings(musicInit);

    if (AK::MusicEngine::Init(&musicInit) != AK_Success)
    {
        assert(!"Could not initialize the Music Engine.");
        return false;
    }

    //

    // Initialize Spatial Audio

    // Using default initialization parameters

    //


    AkSpatialAudioInitSettings settings; // The constructor fills AkSpatialAudioInitSettings with the recommended default settings. 

    if (AK::SpatialAudio::Init(settings) != AK_Success)
    {
        assert(!"Could not initialize the Spatial Audio.");
        return false;
    }

    return true;
}

update_status ModuleAudio::PreUpdate(float dt)
{


	return UPDATE_CONTINUE;
}

update_status ModuleAudio::Update(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleAudio::CleanUp()
{

	return true;
}