#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "C_Camera.h"

#include <string>

#include <iostream>
#include <filesystem>
#include <fstream>

ModuleAudio::ModuleAudio(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

// Destructor
ModuleAudio::~ModuleAudio()
{
}

// Called before render is available
bool ModuleAudio::Init()
{
	bool ret = true;
	InitSoundEngine();


	return ret;
}

bool ModuleAudio::Start()
{

	return true;
}

update_status ModuleAudio::PreUpdate(float dt)
{


	return UPDATE_CONTINUE;
}

update_status ModuleAudio::Update(float dt)
{
	ProcessAudio();

	return UPDATE_CONTINUE;
}

bool ModuleAudio::CleanUp()
{

	AK::SoundEngine::Term();

	if (AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();

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

	AkStreamMgrSettings streamSettings;
	AK::StreamMgr::GetDefaultSettings(streamSettings);

	if (!AK::StreamMgr::Create(streamSettings))
	{
		LOG("Couldn't create the Stream Manager");
		return false;
	}

	AkDeviceSettings devSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(devSettings);

	if (g_lowLevelIO.Init(devSettings) != AK_Success)
	{
		LOG("Couldn't create the streaming device and Low-Level I/O system");
		return false;
	}

	AkInitSettings initSettings;
	AkPlatformInitSettings platfInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platfInitSettings);

	if (AK::SoundEngine::Init(&initSettings, &platfInitSettings) != AK_Success)
	{
		LOG("Couldn't initialize the sound engine");
		return false;
	}

	AkSpatialAudioInitSettings spatialSettings;

	if (AK::SpatialAudio::Init(spatialSettings) != AK_Success)
	{
		LOG("Couldn't initialize the spatial audio");
		return false;
	}


	GetAudioID();

	g_lowLevelIO.SetBasePath(AKTEXT("Assets/Wwise/"));

	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

	AkBankID bankID = 0;
	if (AK::SoundEngine::LoadBank(L"Init.bnk", bankID) != AK_Success)
	{
		LOG("Couldn't find the bank: Init.bnk");
		return false;
	}
	if (AK::SoundEngine::LoadBank(L"ragnarEngine.bnk", bankID) != AK_Success)
	{
		LOG("Couldn't find the bank: ragnarEngine.bnk");
		return false;
	}


	return true;
}

void ModuleAudio::ProcessAudio()
{
	AK::SoundEngine::RenderAudio();
}


void ModuleAudio::RegisterGameObject(unsigned int id)
{
	AK::SoundEngine::RegisterGameObj(id);

}

void ModuleAudio::UnregisterGameObject(unsigned int id)
{
	AK::SoundEngine::UnregisterGameObj(id);
}

void ModuleAudio::SetDefaultListener(const AkGameObjectID id, C_Transform* listenerPosition)
{
	AK::SoundEngine::SetDefaultListeners(&id, MAX_LISTENERS);
	currentListenerPosition = listenerPosition;
}

void ModuleAudio::RemoveDefaultListener(const AkGameObjectID id)
{
	AK::SoundEngine::RemoveDefaultListener(id);
}

void ModuleAudio::AddListeners(unsigned int emitter_id, const AkGameObjectID listener_id)
{
	AK::SoundEngine::SetListeners(emitter_id, &listener_id, MAX_LISTENERS);
}

void ModuleAudio::AddReverbZone(C_ReverbZone* reverbZone)
{
	reverbZones.push_back(reverbZone);
}

void ModuleAudio::DeleteReverbZone(C_ReverbZone* reverbZone)
{
	vector<C_ReverbZone*>::iterator iterator = reverbZones.begin();

	for (; iterator != reverbZones.end(); ++iterator)
	{
		if (*iterator == reverbZone)
		{
			reverbZones.erase(iterator);
			break;
		}
	}
}
void ModuleAudio::CheckReverbGameObject(unsigned int UUID)
{
	AkAuxSendValue aEnvs;
	for (int i = 0; i < reverbZones.size(); ++i)
	{
		if (reverbZones[i]->GetReverbZoneAABB().Contains(currentListenerPosition->getPosition()))
		{
			aEnvs.listenerID = AK_INVALID_GAME_OBJECT;
			aEnvs.auxBusID = AK::SoundEngine::GetIDFromString(reverbZones[i]->GetReverbBusName().c_str());
			aEnvs.fControlValue = 1.5f;

			if (AK::SoundEngine::SetGameObjectAuxSendValues(UUID, &aEnvs, 1) != AK_Success)
			{
				LOG("Couldnt set aux send values");
			}
		}
		else
		{
			aEnvs.listenerID = AK_INVALID_GAME_OBJECT;
			aEnvs.auxBusID = AK::SoundEngine::GetIDFromString(L"Master Audio Bus");
			aEnvs.fControlValue = 1.0f;

			if (AK::SoundEngine::SetGameObjectAuxSendValues(UUID, &aEnvs, 1) != AK_Success)
			{
				LOG("Couldnt set aux send values");
			}
		}
	}
}

void ModuleAudio::SetRTPCValue(const char* event, AkRtpcValue volume, uint id)
{
	// Assuming "Volume" is the RTPC name in Wwise
	AKRESULT result = AK::SoundEngine::SetRTPCValue(event, volume, id);
	if (result != AK_Success)
	{
		LOG("SetRTPCValue failed with error code: %d", result);
	}
	else if( result == AK_InvalidFloatValue)
	{
		LOG("AK_InvalidFloatValue error");
	}
	else if(result == AK_InvalidID)
	{
		LOG("AK_InvalidID error");
	}
}


void ModuleAudio::SetListenerPos(GameObject* listener, unsigned int id)
{
	float3 position = listener->mTransform->mPosition;

	AkSoundPosition listenerPosition;
	listenerPosition.SetOrientation({ 0,0,-1 }, { 0,1,0 });
	listenerPosition.SetPosition(position.x, position.y, position.z);

	AK::SoundEngine::SetPosition(id, listenerPosition);
}

AkPlayingID ModuleAudio::PostEvent(const char* eventName, unsigned int source_id)
{
	AkPlayingID playingID = AK::SoundEngine::PostEvent(eventName, source_id);
	if (playingID == AK_INVALID_PLAYING_ID)
	{
		LOG("Post event %s failed", eventName);
		return -1;
	}
	return playingID;
}

void ModuleAudio::SetSourcePos(GameObject* source, unsigned int id)
{
	float3 pos = source->mTransform->mPosition;

	AkSoundPosition sourcePosition;
	sourcePosition.SetOrientation({ 0,0,-1 }, { 0,1,0 });
	sourcePosition.SetPosition(pos.x, pos.y, pos.z);

	AK::SoundEngine::SetPosition(id, sourcePosition);
}

void ModuleAudio::StopEvent(const char* eventName, unsigned int id)
{
	AK::SoundEngine::ExecuteActionOnEvent(eventName, AK::SoundEngine::AkActionOnEventType_Stop, id);
}

void ModuleAudio::PauseEvent(const char* eventName, unsigned int id)
{
	AK::SoundEngine::ExecuteActionOnEvent(eventName, AK::SoundEngine::AkActionOnEventType_Pause, id);
}

void ModuleAudio::ResumeEvent(const char* eventName, unsigned int id)
{
	AK::SoundEngine::ExecuteActionOnEvent(eventName, AK::SoundEngine::AkActionOnEventType_Resume, id);
}

void ModuleAudio::GetAudioID()
{
	std::ifstream filePath("Assets/Wwise/Wwise_IDs.h");

	std::string line;

	while (std::getline(filePath, line))
	{
		if (line.find("EVENTS") != string::npos)
		{
			while (std::getline(filePath, line))
			{
				if (line.find("}") != std::string::npos)
				{
					break;
				}
				else if (line.find("AkUniqueID") != string::npos)
				{
					line = line.substr(0, line.find("=") - 1);
					line = line.substr(line.find_last_of(" ") + 1, line.length());

					events.push_back(line);
				}
			}
		}
		else if (line.find("BANKS") != string::npos)
		{
			while (std::getline(filePath, line))
			{
				if (line.find("}") != string::npos)
				{
					break;
				}
				else if (line.find("AkUniqueID") != string::npos)
				{
					line = line.substr(0, line.find("=") - 1);
					line = line.substr(line.find_last_of(" ") + 1, line.length());

					soundBanks.push_back(line);
				}
			}
		}
	}
}