#pragma once
#include <iostream>
#include "GL\glew.h"

class Texture
{
public:
	bool LoadTexture2D(std::string path, bool bGenerateMipMaps = false);
	void BindTexture(int textureUnit = 0);

	void SetFiltering(int magnification, int minification);
	void SetSamplerParameter(GLenum parameter, GLenum value);

	int GetMinificationFilter();
	int GetMagnificationFilter();

	void ReleaseTexture();

	std::string GetPath();

	Texture();

private:
	int m_width, m_height, m_BPP;

	unsigned int m_textureID;
	unsigned int m_samplerID;

	bool m_bMipMapsGenerated;
	
	int m_minification, m_magnification;

	std::string m_path;
};

