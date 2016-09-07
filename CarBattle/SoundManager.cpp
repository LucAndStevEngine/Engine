#include "SoundManager.h"
#include "ResourceManager.h"
#include "SceneNode.h"
#define NULL 0

SoundManager::SoundManager() : m_currentChannel(0)
{
}

void SoundManager::Init() 
{
	FMOD::System_Create(&m_system); 
	m_system->init(MAX_CHANNELS, FMOD_INIT_NORMAL, NULL);
	m_system->set3DSettings(10.0f, 10.0f, 10.0f);
	m_system->getMasterChannelGroup(&m_channelGroup);

}

void SoundManager::Shutdown()
{
	delete m_system;
}

bool SoundManager::LoadSound(const char * filePath)
{
	if (ResourceManager::GetSound(filePath) != NULL)
	{
		return true;
	}

	FMOD::Sound* sound;
	FMOD_RESULT result;
	result = m_system->createSound(filePath, FMOD_3D, 0, &sound);
	result = sound->set3DMinMaxDistance(.1f, 1.0f);
	if (result != FMOD_OK)
	{
		std::cout << "Cannot open " << filePath << std::endl;
		return false;
	}

	// Send to the resource manager
	ResourceManager::SaveSound(sound, filePath);
	return true;
}

void SoundManager::PlaySound(const char * filePath, bool loop)
{
	// Load from resource manager
	FMOD::Sound* sound = ResourceManager::GetSound(filePath);

	if (!sound)
	{
		if (!LoadSound(filePath))
		{
			return;
		}
		sound = ResourceManager::GetSound(filePath);
	}	

	if (!loop)
	{
		sound->setMode(FMOD_LOOP_OFF);
	}
	else
	{
		sound->setMode(FMOD_LOOP_NORMAL);
		sound->setLoopCount(-1);
	}

	m_system->playSound(sound, NULL, false, &m_channels[m_currentChannel]);
	m_channels[m_currentChannel]->setVolume(0.1f);
	m_currentChannel++;
}

void SoundManager::PlaySound(FMOD::Sound* sound, bool loop)
{
	if (!loop)
	{
		sound->setMode(FMOD_LOOP_OFF);
	}
	else
	{
		sound->setMode(FMOD_LOOP_NORMAL);
		sound->setLoopCount(-1);
	}

	m_system->playSound(sound, NULL, false, &m_channels[m_currentChannel]);
	m_channels[m_currentChannel]->setVolume(0.1f);
	//m_currentChannel++;

}

void SoundManager::Play3DSound(FMOD::Sound * sound, SceneNode * source, SceneNode * listener, bool loop)
{
	if (!loop)
	{
		sound->setMode(FMOD_LOOP_OFF);
	}
	else
	{
		sound->setMode(FMOD_LOOP_NORMAL);
		sound->setLoopCount(-1);
	}
	FMOD_3D_ATTRIBUTES temp = get3dAttribute(source);
	FMOD_3D_ATTRIBUTES temp2 = get3dAttribute(listener);

//	m_system->set3DListenerAttributes(0, &temp2.position, &temp2.velocity, &temp2.forward, &temp2.up );
	m_system->playSound(sound, m_channelGroup, false, &m_channels[m_currentChannel]);
	m_system->set3DListenerAttributes(0, &temp2.position, &temp2.velocity, &temp2.forward, &temp2.up);
	m_channels[m_currentChannel]->set3DAttributes(&temp.position, &temp.velocity);
//	m_channels[m_currentChannel]->setFrequency(1);
	m_channels[m_currentChannel]->setVolume(0.1f);
	//m_currentChannel++;
	
}



void SoundManager::ReleaseSound(const char * filePath)
{
	
}

void SoundManager::ReleaseSound(FMOD::Sound * sound)
{
}

FMOD_3D_ATTRIBUTES SoundManager::get3dAttribute(SceneNode * sceneNode)
{
	FMOD_3D_ATTRIBUTES temp;
	temp.forward = { sceneNode->transform.forward.x, sceneNode->transform.forward.y, sceneNode->transform.forward.z };
	temp.position = { sceneNode->transform.position.x, sceneNode->transform.position.y, sceneNode->transform.position.z };
	temp.up = { sceneNode->transform.up.x, sceneNode->transform.up.y, sceneNode->transform.up.z };
	temp.velocity = { 0,0,0};
	return temp;
}
