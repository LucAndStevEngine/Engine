#pragma once
#include "VertexBufferObject.h"
#include "Texture.h"
#include "Shader.h"
#include <vector>

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
	
private:

	bool m_bIsLoaded;
	static VertexBufferObject m_vboModelData;
	static unsigned int m_VAO;
	static std::vector<Texture*> m_textures;
	std::vector<int> m_meshStartIndices;
	std::vector<int> m_meshSizes;
	std::vector<int> m_materialIndices;
	int numMaterials;
};

