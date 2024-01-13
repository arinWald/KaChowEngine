#pragma once

#include "Module.h"
#include "Globals.h"
#include "C_ReverbZone.h"
//#include "AudioEvent.h"


#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>  
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>
#include <AK/SoundEngine/Common/IAkPlugin.h>
#include <AK/SoundEngine/Win32/AkFilePackageLowLevelIOBlocking.h>

#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif



#define MAX_LISTENERS 1

class AudioEvent;
class ModuleAudio : public Module
{
public:

	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	bool CleanUp();

	bool InitSoundEngine();

	void ProcessAudio();

	void RegisterGameObject(unsigned int id);
	void UnregisterGameObject(unsigned int id);


	AkPlayingID PostEvent(const char* eventName, unsigned int source_id);
	void StopEvent(const char* event, unsigned int id);
	void PauseEvent(const char* event, unsigned int id);
	void ResumeEvent(const char* event, unsigned int id);

	void SetRTPCValue(const char* event, AkRtpcValue volume, uint id);

	void SetDefaultListener(const AkGameObjectID id);
	void RemoveDefaultListener(const AkGameObjectID id);
	void AddListeners(unsigned int emitter_id, const AkGameObjectID listener_id);

	void AddReverbZone(C_ReverbZone* reverbZone);
	void DeleteReverbZone(C_ReverbZone* reverbZone);

	void SetListenerPos(GameObject* listener, unsigned int id);
	void SetSourcePos(GameObject* source, unsigned int id);

	void GetAudioID();

	std::vector<std::string> events;
	std::vector<std::string> soundBanks;

private:
	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

	vector<C_ReverbZone*> reverbZones;

};

