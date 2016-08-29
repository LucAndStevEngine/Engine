#pragma once

#include "fmod\fmod.hpp"
#include "fmod\fmod_errors.h"

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
	void ReleaseSound(const char* filePath);
	void ReleaseSound(FMOD::Sound* sound);
	
private:
	FMOD::Channel* m_channels[MAX_CHANNELS];
	int m_currentChannel;
	FMOD::System *m_system;
};

