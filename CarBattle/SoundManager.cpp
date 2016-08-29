#include "SoundManager.h"
#include "ResourceManager.h"

#define NULL 0

SoundManager::SoundManager() : m_currentChannel(0)
{
}

void SoundManager::Init() 
{
	FMOD::System_Create(&m_system); 
	m_system->init(MAX_CHANNELS, FMOD_INIT_NORMAL, NULL);

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
	result = m_system->createSound(filePath, FMOD_LOOP_OFF, 0, &sound);
	
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
	m_currentChannel++;

}

void SoundManager::ReleaseSound(const char * filePath)
{
	
}

void SoundManager::ReleaseSound(FMOD::Sound * sound)
{
}
