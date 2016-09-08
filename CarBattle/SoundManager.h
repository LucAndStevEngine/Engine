#pragma once

#include "fmod\fmod.hpp"
#include "fmod\fmod_errors.h"
#include "fmod\fmod_common.h"
#include <vector>
#define MAX_CHANNELS 32
class SceneNode;
class SoundManager
{
public:
	SoundManager();

	void Init();
	void Shutdown();
	void Update();
	bool LoadSound(const char* filePath);
	void PlaySound(const char* filePath, const SceneNode* source, bool loop = false);
	void PlaySound(FMOD::Sound* sound, const SceneNode* source, bool loop = false);
	void PlaySound(const char* filePath, bool loop = false);
	void PlaySound(FMOD::Sound* sound, bool loop = false);
	void Play3DSound(FMOD::Sound* sound, const SceneNode* source, const SceneNode* listener, bool loop = false);
	void ReleaseSound(const char* filePath);
	void ReleaseSound(FMOD::Sound* sound);
	void AddListener(SceneNode* listener);
	void RemoveListener(SceneNode* listener);
private:

	void nextChannel();
	std::vector<SceneNode*> m_listeners;
	FMOD_3D_ATTRIBUTES get3dAttribute(const SceneNode* sceneNode);
	FMOD::Channel* m_channels[MAX_CHANNELS];
	FMOD::ChannelGroup * m_channelGroup;
	int m_currentChannel;
	FMOD::System *m_system;
};

