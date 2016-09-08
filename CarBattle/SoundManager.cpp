#include "SoundManager.h"
#include "ResourceManager.h"
#include "SceneNode.h"
#include "ListenerComponent.h"
#define NULL 0

SoundManager::SoundManager() : m_currentChannel(0)
{
}

void SoundManager::Init() 
{
	FMOD::System_Create(&m_system); 
	m_system->init(MAX_CHANNELS, FMOD_INIT_NORMAL, NULL);
	m_system->set3DSettings(1.0f, 1.0f, 1.0f);
	m_system->getMasterChannelGroup(&m_channelGroup);

}

void SoundManager::Shutdown()
{
	delete m_system;
}

void SoundManager::Update()
{
	m_system->update();
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
	result = sound->set3DMinMaxDistance(1.0f, 10000.0f);
	if (result != FMOD_OK)
	{
		std::cout << "Cannot open " << filePath << std::endl;
		return false;
	}

	// Send to the resource manager
	ResourceManager::SaveSound(sound, filePath);
	return true;
}

void SoundManager::PlaySound(const char * filePath, const SceneNode * source, bool loop)
{
	FMOD::Sound* sound = ResourceManager::GetSound(filePath);

	if (!sound)
	{
		if (!LoadSound(filePath))
		{
			return;
		}
		sound = ResourceManager::GetSound(filePath);
	}
	PlaySound(sound, source, loop);
}

void SoundManager::PlaySound(FMOD::Sound * sound, const SceneNode * source, bool loop)
{
	
	float distance = -1;
	int closestListener = -1;
	for (int i = 0; i < m_listeners.size(); i++)
	{
		if (!m_listeners[i]->GetComponent<ListenerComponent>()->isEnable)
			continue;

		float newDist = glm::distance2(m_listeners[i]->transform.position, source->transform.position);
		if (distance < 0 || newDist < distance)
		{
			distance = newDist;
			closestListener = i;
		}
	}

	if (closestListener <= -1)
	{
		std::cout << "no listeners found" << std::endl;
		return;
	}

	Play3DSound(sound, source, m_listeners[closestListener], loop);
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

	PlaySound(sound, loop);
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
	nextChannel();

}

void SoundManager::Play3DSound(FMOD::Sound * sound, const SceneNode * source, const SceneNode * listener, bool loop)
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

	m_system->playSound(sound, m_channelGroup, false, &m_channels[m_currentChannel]);
	m_system->set3DListenerAttributes(0, &temp2.position, &temp2.velocity, &temp2.forward, &temp2.up);
	m_channels[m_currentChannel]->set3DAttributes(&temp.position, &temp.velocity);
//	m_channels[m_currentChannel]->setFrequency(1);
	m_channels[m_currentChannel]->setVolume(0.1f);
	nextChannel();
}



void SoundManager::ReleaseSound(const char * filePath)
{
	
}

void SoundManager::ReleaseSound(FMOD::Sound * sound)
{
}

void SoundManager::AddListener(SceneNode* listener)
{
	m_listeners.push_back(listener);
}

void SoundManager::RemoveListener(SceneNode* listener)
{
	for (int i = 0; i < m_listeners.size(); i++)
	{
		if (m_listeners[i] == listener)
		{
			m_listeners.erase(m_listeners.begin() + i);
			return;
		}
	}
}

void SoundManager::nextChannel()
{
	m_currentChannel++;
	if (m_currentChannel >= MAX_CHANNELS)
		m_currentChannel = 0;
}

FMOD_3D_ATTRIBUTES SoundManager::get3dAttribute(const SceneNode * sceneNode)
{
	FMOD_3D_ATTRIBUTES temp;
	temp.forward = { sceneNode->transform.forward.x, sceneNode->transform.forward.y, sceneNode->transform.forward.z };
	temp.position = { sceneNode->transform.position.x, sceneNode->transform.position.y, sceneNode->transform.position.z };
	temp.up = { sceneNode->transform.up.x, sceneNode->transform.up.y, sceneNode->transform.up.z };
	temp.velocity = { 0,0,0};
	return temp;
}
