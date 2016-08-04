#include "FrameBuffer.h"



FrameBuffer::FrameBuffer()
{
	m_frameBufferID = 0;
	m_depthRenderBufferID = 0;
}

bool FrameBuffer::CreateFrameBufferWithTexture(int width, int height)
{
	if (m_frameBufferID != 0)
	{
		return false;
	}

	glGenFramebuffers(1, &m_frameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);

	m_frameBufferTex.CreateEmptyTexture(width, height, 24, GL_RGB);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_frameBufferTex.GetTextureID(), 0);

	m_width = width;
	m_height = height;

	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

bool FrameBuffer::AddDepthBufer()
{
	if (m_frameBufferID == 0)
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);

	glGenRenderbuffers(1, &m_depthRenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBufferID);

	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void FrameBuffer::BindFrameBuffer(bool bSetFullViewport)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);
	if (bSetFullViewport)
	{
		glViewport(0, 0, m_width, m_height);
	}
}

void FrameBuffer::SetFrameBufferTextureFiltering(int magnification, int minification)
{
	m_frameBufferTex.SetFiltering(magnification, minification);
}

void FrameBuffer::BindFrameBufferTexture(int textureUnit, bool bRegenMipMaps)
{
	m_frameBufferTex.BindTexture(textureUnit);
	if (bRegenMipMaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

glm::mat4 FrameBuffer::CalculateProjectionMatrix(float FOV, float near, float far)
{
	return glm::perspective(FOV, float(m_width) / float(m_height), near, far);
}
glm::mat4 FrameBuffer::CalculateOrthoMatrix()
{
	return glm::ortho(0.0f, float(m_width), 0.0f, float(m_height));
}

void FrameBuffer::DeleteFrameBuffer()
{
	if (m_frameBufferID)
	{
		glDeleteFramebuffers(1, &m_frameBufferID);
		m_frameBufferID = 0;
	}
	if (m_depthRenderBufferID)
	{
		glDeleteRenderbuffers(1, &m_depthRenderBufferID);
		m_depthRenderBufferID = 0;
	}
	m_frameBufferTex.ReleaseTexture();
}

int FrameBuffer::GetWidth()
{
	return m_width;
}
int FrameBuffer::GetHeight()
{
	return m_height;
}