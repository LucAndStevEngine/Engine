#include "Skybox.h"

#include "glm\glm.hpp"

void Skybox::LoadSkybox(std::string directory, std::string front, std::string back, std::string left, std::string right, std::string top, std::string bottom)
{
	m_Textures[0].LoadTexture2D(directory + front, true);
	m_Textures[1].LoadTexture2D(directory + back, true);
	m_Textures[2].LoadTexture2D(directory + left, true);
	m_Textures[3].LoadTexture2D(directory + right, true);
	m_Textures[4].LoadTexture2D(directory + top, true);
	m_Textures[5].LoadTexture2D(directory + bottom, true);

	m_directory = directory;

	m_front = front;
	m_back = back;
	m_left = left;
	m_right = right;
	m_top = top;
	m_bottom = bottom;

	for (int i = 0; i < 6; i++)
	{
		m_Textures[i].SetFiltering(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		m_Textures[i].SetSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		m_Textures[i].SetSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}


	glm::vec3 vSkyBoxVertices[24] =
	{
		// Front face
		glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(50.0f, -50.0f, 50.0f), glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, 50.0f),
		// Back face
		glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec3(50.0f, 50.0f, -50.0f), glm::vec3(50.0f, -50.0f, -50.0f),
		// Left face
		glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, 50.0f), glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(-50.0f, -50.0f, -50.0f),
		// Right face
		glm::vec3(50.0f, 50.0f, -50.0f), glm::vec3(50.0f, -50.0f, -50.0f), glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(50.0f, -50.0f, 50.0f),
		// Top face
		glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(50.0f, 50.0f, -50.0f), glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(50.0f, 50.0f, 50.0f),
		// Bottom face
		glm::vec3(50.0f, -50.0f, -50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec3(50.0f, -50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, 50.0f),
	};
	glm::vec2 vSkyBoxTexCoords[4] =
	{
		glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)
	};

	glm::vec3 vSkyBoxNormals[6] =
	{
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};


	for (int i = 0; i < 24; i++)
	{
		m_vboRenderData.AddData(&vSkyBoxVertices[i], sizeof(glm::vec3));
		m_vboRenderData.AddData(&vSkyBoxTexCoords[i % 4], sizeof(glm::vec2));
		m_vboRenderData.AddData(&vSkyBoxNormals[i / 4], sizeof(glm::vec3));
	}


	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	m_vboRenderData.CreateVBO();
	m_vboRenderData.BindVBO();

	m_vboRenderData.UploadDataToGPU(GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glBindVertexArray(0);
}

void Skybox::Render()
{
	glDepthMask(0);
	glBindVertexArray(m_VAO);
	for (int i = 0; i < 6; i++)
	{
		m_Textures[i].BindTexture();
		glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
	}
	glBindVertexArray(0);
	glDepthMask(1);
}

void Skybox::ReleaseSkybox()
{

	for (int i = 0; i < 6; i++)
	{
		m_Textures[i].ReleaseTexture();
	}
	glDeleteVertexArrays(1, &m_VAO);
	m_vboRenderData.ReleaseVBO();
}
