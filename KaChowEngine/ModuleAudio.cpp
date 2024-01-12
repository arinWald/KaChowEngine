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

    AkStreamMgrSettings stmSettings;
    AK::StreamMgr::GetDefaultSettings(stmSettings);

    if (!AK::StreamMgr::Create(stmSettings))
    {
        assert(!"Could not create the Streaming Manager");
        return false;
    }

    AkDeviceSettings deviceSettings;
    AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

    if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
    {
        assert(!"Could not create the streaming device and Low-Level I/O system");
        return false;
    }

    AkInitSettings          initSettings;
    AkPlatformInitSettings  platformInitSettings;

    AK::SoundEngine::GetDefaultInitSettings(initSettings);
    AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

    initSettings.uMaxNumPaths = 16;

    if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
    {
        return false;
    }

    AkMusicSettings musicInit;
    AK::MusicEngine::GetDefaultInitSettings(musicInit);

    if (AK::MusicEngine::Init(&musicInit) != AK_Success)
    {
        assert(!"Could not initialize the Music Engine.");
        return false;
    }

    AkSpatialAudioInitSettings settings;

    if (AK::SpatialAudio::Init(settings) != AK_Success)
    {
        assert(!"Could not initialize the Spatial Audio.");
        return false;
    }

    g_lowLevelIO.SetBasePath(AKTEXT("Assets/Wwise/"));

    AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

    AkBankID bankID;
    if (AK::SoundEngine::LoadBank(L"Init.bnk", bankID) != AK_Success)
    {
        LOG("Couldn't find the bank: Init.bnk");
        return false;
    }
    if (AK::SoundEngine::LoadBank(L"Main.bnk", bankID) != AK_Success)
    {
        LOG("Couldn't find the bank: Main.bnk");
        return false;
    }

    return true;
}

void ModuleAudio::ProcessAudio()
{
    AK::SoundEngine::RenderAudio();
}

void ModuleAudio::TermSoundEngine()
{
    AK::SoundEngine::Term();;

    if (AK::IAkStreamMgr::Get())
        AK::IAkStreamMgr::Get()->Destroy();
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
    //TermSoundEngine();
	return true;
}