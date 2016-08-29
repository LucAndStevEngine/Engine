#include "ResourceManager.h"

std::map<std::string, ShaderProgram*> ResourceManager::Shaders;
std::map<std::string, FMOD::Sound*> ResourceManager::Sounds;

ResourceManager::ResourceManager()
{
}

ShaderProgram* ResourceManager::SaveShader(ShaderProgram* shader, std::string name)
{
	Shaders[name] = shader;
	return shader;
}

ShaderProgram* ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}


void ResourceManager::Clear()
{
	for (auto iter : Shaders)
	{
		glDeleteProgram(iter.second->GetProgramID());
	}		
}

void ResourceManager::RemoveShader(std::string name)
{
	ShaderProgram* shader = GetShader(name);
	glDeleteProgram(shader->GetProgramID());
	Shaders.erase(name);
}

FMOD::Sound * ResourceManager::SaveSound(FMOD::Sound* sound, std::string path)
{
	Sounds[path] = sound;
	return sound;
}

FMOD::Sound * ResourceManager::GetSound(std::string path)
{
	return Sounds[path];
}

void ResourceManager::RemoveSound(std::string path)
{
	FMOD::Sound* sound = GetSound(path);
	Sounds.erase(path);
	delete sound;
}
