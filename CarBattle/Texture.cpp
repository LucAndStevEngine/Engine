#include "Texture.h"
#include "Time.h"
#include "GL\glew.h"
#include "freeImage\FreeImage.h"

Texture::Texture()
{
	m_bMipMapsGenerated = false;
}

void Texture::CreateEmptyTexture(int width, int height, int BPP, GLenum format)
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	if (format == GL_RGBA || format == GL_BGRA)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
	}
	else if (format == GL_RGB || format == GL_BGR)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
	}

	glGenSamplers(1, &m_samplerID);
}

void Texture::CreateFromData(BYTE* data, int width, int height, int BPP, GLenum format, bool bGenerateMipMaps)
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	if (format == GL_RGBA || format == GL_BGRA)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	}
	else if (format == GL_RGB || format == GL_BGR)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	}

	if (bGenerateMipMaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glGenSamplers(1, &m_samplerID);

	m_path = "";
	m_bMipMapsGenerated = bGenerateMipMaps;
	m_width = width;
	m_height = height;
	m_BPP = BPP;
}


bool Texture::LoadTexture2D(std::string path, bool bGenerateMipMaps)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	FIBITMAP* dib(0);

	// Try to get the file type based off the file extension
	fif = FreeImage_GetFileType(path.c_str(), 0);

	// If the filetype is unknown try to get the filetype from the path
	if (fif == FIF_UNKNOWN)
	{
		fif = FreeImage_GetFIFFromFilename(path.c_str());
	}

	// if still unkown then return false
	if (fif == FIF_UNKNOWN)
	{
		std::cout << "Unknown filetype" << std::endl;
		return false;
	}

	// Check to see if the plugin can read the file type
	if (FreeImage_FIFSupportsReading(fif))
	{
		dib = FreeImage_Load(fif, path.c_str());
	}
	if (!dib)
	{
		std::cout << "Cannot read filetype at - " << path.c_str() << std::endl;
		return false;
	}

	// Get the data from the image
	BYTE* dataPointer = FreeImage_GetBits(dib);

	m_width = FreeImage_GetWidth(dib);
	m_height = FreeImage_GetHeight(dib);
	m_BPP = FreeImage_GetBPP(dib);

	// if one of these fail, return false
	if (dataPointer == NULL || m_width == 0 || m_height == 0)
	{
		std::cout << "Data failed to read" << std::endl;
		return false;
	}

	// Generate open gl textureID for the texture
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	int format = m_BPP == 24 ? GL_BGR : m_BPP == 8 ? GL_LUMINANCE : 0;
	int internalFormat = m_BPP == 24 ? GL_RGB : GL_DEPTH_COMPONENT;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, dataPointer);

	if (bGenerateMipMaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	FreeImage_Unload(dib);

	glGenSamplers(1, &m_samplerID);

	m_path = path;
	m_bMipMapsGenerated = bGenerateMipMaps;

	return true;
}

void Texture::BindTexture(int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTextures(0, 1, &m_textureID);
	glBindSamplers(0, 1, &m_samplerID);
}

void Texture::SetFiltering(int magnification, int minification)
{
	// Set magnification filter
	if (magnification == TEXTURE_FILTER_MAG_NEAREST)
		glSamplerParameteri(m_samplerID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else if (magnification == TEXTURE_FILTER_MAG_BILINEAR)
		glSamplerParameteri(m_samplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set minification filter
	if (minification == TEXTURE_FILTER_MIN_NEAREST)
		glSamplerParameteri(m_samplerID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if (minification == TEXTURE_FILTER_MIN_BILINEAR)
		glSamplerParameteri(m_samplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if (minification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)
		glSamplerParameteri(m_samplerID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if (minification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
		glSamplerParameteri(m_samplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if (minification == TEXTURE_FILTER_MIN_TRILINEAR)
		glSamplerParameteri(m_samplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	m_minification = minification;
	m_magnification = magnification;
}

void Texture::SetSamplerParameter(GLenum parameter, GLenum value)
{
	glSamplerParameteri(m_samplerID, parameter, value);
}

int Texture::GetMinificationFilter()
{
	return m_minification;
}
int Texture::GetMagnificationFilter()
{

	return m_magnification;
}

void Texture::ReleaseTexture()
{
	glDeleteSamplers(1, &m_samplerID);
	glDeleteTextures(1, &m_textureID);
}


unsigned int Texture::GetTextureID()
{
	return m_textureID;
}

std::string Texture::GetPath()
{
	return m_path;
}