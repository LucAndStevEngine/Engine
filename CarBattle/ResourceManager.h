#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string.h>

#include "GL/glew.h"

#include "Shader.h"
#include "fmod\fmod.hpp"

class ResourceManager
{
public:
	// Resource storage
	static std::map<std::string, ShaderProgram*> Shaders;
	static std::map<std::string, Shader*> IndividualShader;
	static std::map<std::string, FMOD::Sound*> Sounds;

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
	// Removes a shader from the map with the specified name
	static void RemoveShader(std::string name);

	static FMOD::Sound* SaveSound(FMOD::Sound* sound, std::string path);
	static FMOD::Sound* GetSound(std::string path);
	static void RemoveSound(std::string path);

	// Properly de-allocates all loaded resources
	static void Clear();

private:
	ResourceManager();

};

#endif

