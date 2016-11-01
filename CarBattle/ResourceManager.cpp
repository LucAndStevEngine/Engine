#include "ResourceManager.h"

std::map<std::string, ShaderProgram*> ResourceManager::Shaders;
std::map<std::string, Shader*> ResourceManager::IndividualShader;
std::map<std::string, FMOD::Sound*> ResourceManager::Sounds;
std::map<std::string, AssimpModel*> ResourceManager::Models;

Shader* ResourceManager::LoadShader(char* path, int type)
{
	Shader* shader = ResourceManager::GetIndividualShader(path);
	if (!shader)
	{
		shader = new Shader();
		shader->LoadShader(path, type);
		ResourceManager::SaveIndividualShader(shader, path);
	}

	return shader;
}

ResourceManager::ResourceManager()
{
}

Shader * ResourceManager::SaveIndividualShader(Shader *shader, std::string name)
{
	IndividualShader[name] = shader;
	return shader;
}

Shader * ResourceManager::GetIndividualShader(std::string name)
{
	return IndividualShader[name];
}

void ResourceManager::RemoveIndividualShader(std::string name)
{
	Shader* shader = GetIndividualShader(name);
	Shaders.erase(name);
}

ShaderProgram* ResourceManager::SaveShader(ShaderProgram* shader, std::string name)
{
	Shaders[name] = shader;
	return shader;
}

ShaderProgram* ResourceManager::GetShader(char* fragment, char* geometry, char* vertex, char* name)
{
	ShaderProgram* program = GetShader(name);
	if (program)
	{
		return program;
	}

	program = new ShaderProgram();
	program->CreateProgram();

	if (fragment != NULL)
	{
		Shader* shader = LoadShader(fragment, GL_FRAGMENT_SHADER);
		program->AddShaderToProgram(shader);
	}
	if (geometry != NULL)
	{
		Shader* shader = LoadShader(geometry, GL_GEOMETRY_SHADER);
		program->AddShaderToProgram(shader);
	}
	if (vertex != NULL)
	{
		Shader* shader = LoadShader(vertex, GL_VERTEX_SHADER);
		program->AddShaderToProgram(shader);
	}

	program->LinkProgram();
	ResourceManager::SaveShader(program, name);
	return program;


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

AssimpModel* ResourceManager::SaveModel(AssimpModel * model, std::string path)
{
	Models[path] = model;
	return model;
}

AssimpModel* ResourceManager::GetModel(std::string path)
{
	return Models[path];
}

void ResourceManager::RemoveModel(std::string path)
{
	AssimpModel* model = GetModel(path);
	Models.erase(path);
	delete model;
}
