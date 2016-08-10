#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string.h>

#include "GL/glew.h"

#include "Shader.h"

class ResourceManager
{
public:
	// Resource storage
	static std::map<std::string, ShaderProgram> Shaders;

	// Saves the shader in a map
	static ShaderProgram SaveShader(ShaderProgram, std::string name);
	// Retrieves a stored sader
	static ShaderProgram GetShader(std::string name);
	// Properly de-allocates all loaded resources
	static void Clear();
	// Removes a shader from the map with the specified name
	static void RemoveShader(std::string name);

private:
	ResourceManager();

};

#endif

