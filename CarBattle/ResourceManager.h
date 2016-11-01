#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string.h>

#include "GL/glew.h"

#include "Shader.h"
#include "fmod\fmod.hpp"
#include "AssimpModel.h"

class ResourceManager
{
public:
	// Resource storage
	static std::map<std::string, ShaderProgram*> Shaders;
	static std::map<std::string, Shader*> IndividualShader;
	static std::map<std::string, FMOD::Sound*> Sounds;
	static std::map<std::string, AssimpModel*> ResourceManager::Models;

	// Saves the shader in a map
	static Shader* SaveIndividualShader(Shader*, std::string name);
	// Retrieves a stored sader
	static Shader* GetIndividualShader(std::string name);
	// Removes a shader from the map with the specified name
	static void RemoveIndividualShader(std::string name);

	// Saves the shader in a map
	static ShaderProgram* SaveShader(ShaderProgram*, std::string name);
	// Retrieves a stored sader
	static ShaderProgram* GetShader(std::string name);
	static ShaderProgram* GetShader(char* fragment, char* geometry, char* vertex, char* name);
	// Removes a shader from the map with the specified name
	static void RemoveShader(std::string name);

	static FMOD::Sound* SaveSound(FMOD::Sound* sound, std::string path);
	static FMOD::Sound* GetSound(std::string path);
	static void RemoveSound(std::string path);

	static AssimpModel* SaveModel(AssimpModel* model, std::string path);
	static AssimpModel* GetModel(std::string path);
	static void RemoveModel(std::string path);


	// Properly de-allocates all loaded resources
	static void Clear();

private:
	static Shader* LoadShader(char* path, int type);
	ResourceManager();

};

#endif

