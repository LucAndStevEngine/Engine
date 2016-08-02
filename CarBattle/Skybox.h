#pragma once

#include <iostream>
#include "VertexBufferObject.h"
#include "Texture.h"

class Skybox
{
public:

	void LoadSkybox(std::string directory, std::string front, std::string back, std::string left, std::string right, std::string top, std::string bottom);

	void Render();
	void ReleaseSkybox();

private:
	unsigned int m_VAO;
	class VertexBufferObject m_vboRenderData;
	class Texture m_Textures[6];
	std::string m_directory;
	std::string m_front, m_back, m_left, m_right, m_top, m_bottom;
	
};

