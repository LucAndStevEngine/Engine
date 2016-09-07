#pragma once

#include "fmod\fmod.hpp"
#include "fmod\fmod_errors.h"
#include "fmod\fmod_common.h"
#define MAX_CHANNELS 32

class SoundManager
{
public:
	SoundManager();

	void Init();
	void Shutdown();

	bool LoadSound(const char* filePath);
	void PlaySound(const char* filePath, bool loop = false);
	void PlaySound(FMOD::Sound* sound, bool loop = false);
	void Play3DSound(FMOD::Sound* sound, class SceneNode* source, class SceneNode* listener, bool loop = false);
	void ReleaseSound(const char* filePath);
	void ReleaseSound(FMOD::Sound* sound);
	
private:
	FMOD_3D_ATTRIBUTES get3dAttribute(class SceneNode* sceneNode);
	FMOD::Channel* m_channels[MAX_CHANNELS];
	FMOD::ChannelGroup * m_channelGroup;
	int m_currentChannel;
	FMOD::System *m_system;
};

