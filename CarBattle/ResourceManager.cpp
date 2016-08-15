#include "ResourceManager.h"

std::map<std::string, ShaderProgram*> ResourceManager::Shaders;

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
