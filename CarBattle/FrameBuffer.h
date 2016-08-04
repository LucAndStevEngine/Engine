#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "Texture.h"

class FrameBuffer
{
public:
	bool CreateFrameBufferWithTexture(int width, int height);

	bool AddDepthBufer();
	void BindFrameBuffer(bool bSetFullViewport = true);

	void SetFrameBufferTextureFiltering(int magnification, int minification);
	void BindFrameBufferTexture(int textureUnit = 0, bool bRegenMipMaps = false);
	
	glm::mat4 CalculateProjectionMatrix(float FOV, float near, float far);
	glm::mat4 CalculateOrthoMatrix();

	void DeleteFrameBuffer();

	int GetWidth();
	int GetHeight();

	FrameBuffer();

private:
	int m_width, m_height;
	unsigned int m_frameBufferID;
	unsigned int m_depthRenderBufferID;
	Texture m_frameBufferTex;
};

