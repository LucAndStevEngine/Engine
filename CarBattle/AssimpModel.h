#pragma once
#include "VertexBufferObject.h"
#include "Texture.h"
#include "Shader.h"
#include <vector>


enum TextureType
{
	DIFFUSE = 0,
	SPECULAR = 2,
};

struct TextureData
{
	Texture texture;
	TextureType type;
};

class AssimpModel
{
public:
	bool LoadModelFromFile(char* filePath);
	bool LoadModelFromFileBumpMap(char* filePath, char* colorMap, char* normalMap);

	static void FinalizeVBO();
	static void BindModelsVAO();

	void RenderModel();
	void RenderModelBumpMap(ShaderProgram* program);

	AssimpModel();
	unsigned int GetVAO();

private:

	bool m_bIsLoaded;
	static VertexBufferObject m_vboModelData;
	static unsigned int m_VAO;
	static std::vector<TextureData> m_textures;
	std::vector<int> m_meshStartIndices;
	std::vector<int> m_meshSizes;
	std::vector<int> m_materialIndices;
	int numMaterials;
};

