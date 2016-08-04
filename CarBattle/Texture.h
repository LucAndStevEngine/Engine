#pragma once
#include <iostream>
#include "GL\glew.h"

typedef unsigned char BYTE;

enum TextureFiltering
{
	TEXTURE_FILTER_MAG_NEAREST = 0, // Nearest criterion for magnification
	TEXTURE_FILTER_MAG_BILINEAR, // Bilinear criterion for magnification
	TEXTURE_FILTER_MIN_NEAREST, // Nearest criterion for minification
	TEXTURE_FILTER_MIN_BILINEAR, // Bilinear criterion for minification
	TEXTURE_FILTER_MIN_NEAREST_MIPMAP, // Nearest criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_BILINEAR_MIPMAP, // Bilinear criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_TRILINEAR, // Bilinear criterion for minification on two closest mipmaps, then averaged
};

class Texture
{
public:

	void CreateEmptyTexture(int width, int height, int BPP, GLenum format);
	void CreateFromData(BYTE* data, int width, int height, int BPP, GLenum format, bool bGenerateMipMaps = false);

	bool LoadTexture2D(std::string path, bool bGenerateMipMaps = false);
	void BindTexture(int textureUnit = 0);

	void SetFiltering(int magnification, int minification);
	void SetSamplerParameter(GLenum parameter, GLenum value);

	int GetMinificationFilter();
	int GetMagnificationFilter();

	void ReleaseTexture();

	std::string GetPath();
	unsigned int GetTextureID();

	Texture();

private:
	int m_width, m_height, m_BPP;

	unsigned int m_textureID;
	unsigned int m_samplerID;

	bool m_bMipMapsGenerated;
	
	int m_minification, m_magnification;

	std::string m_path;
};

