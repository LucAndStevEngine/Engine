#include "AssimpModel.h"
#include "Shader.h"

#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"      
#include "assimp/postprocess.h"


VertexBufferObject AssimpModel::m_vboModelData;
unsigned int  AssimpModel::m_VAO;
std::vector<TextureData> AssimpModel::m_textures;

std::string GetDirectoryPath(std::string filePath)
{
	// Get directory path
	std::string directory = "";
	for (int i = filePath.size(); i >= 0; i--)
	{
		if (filePath[i] == '\\' || filePath[i] == '/')
		{
			directory = filePath.substr(0, i + 1);
			break;
		}
	}
	return directory;
}


bool AssimpModel::LoadModelFromFile(char* filePath)
{
	if (m_vboModelData.GetBuffer() == 0)
	{
		m_vboModelData.CreateVBO();
		m_textures.reserve(50);
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene)
	{
		std::cout << "Couldn't load model - Error importing assets" << std::endl;
		return false;
	}

	const int vertexTotalSize = sizeof(aiVector3D) * 2 + sizeof(aiVector2D) + sizeof(aiColor4D);

	int totalVertices = 0;

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		int meshFaces = mesh->mNumFaces;
		m_materialIndices.push_back(mesh->mMaterialIndex);
		int sizeBefore = m_vboModelData.GetCurrentSize();
		m_meshStartIndices.push_back(sizeBefore / vertexTotalSize);

		for (int j = 0; j < meshFaces; j++)
		{
			const aiFace& face = mesh->mFaces[j];
			for (int k = 0; k < face.mNumIndices; k++)
			{
			
				aiVector3D pos = mesh->mVertices[face.mIndices[k]];
				aiVector3D uv;
				if (mesh->HasTextureCoords(0))
				{
					uv = mesh->mTextureCoords[0][face.mIndices[k]];
				}	

				aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);
				aiColor4D color = aiColor4D(1, 1, 1, 1);
				if (AI_SUCCESS == aiGetMaterialColor(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_COLOR_DIFFUSE, &color))
				{

				}

				m_vboModelData.AddData(&pos, sizeof(aiVector3D));
				m_vboModelData.AddData(&uv, sizeof(aiVector2D));
				m_vboModelData.AddData(&normal, sizeof(aiVector3D));
				m_vboModelData.AddData(&color, sizeof(aiColor4D));
			}
		}
		int iMeshVertices = mesh->mNumVertices;
		totalVertices += iMeshVertices;
		m_meshSizes.push_back((m_vboModelData.GetCurrentSize() - sizeBefore) / vertexTotalSize);
	}

	numMaterials = scene->mNumMaterials;

	std::vector<int> materialRemap(numMaterials);

	for (int i = 0; i < numMaterials; i++)
	{
		const aiMaterial* material = scene->mMaterials[i];
		int texIndex = 0;
		aiString path;  // filename
		bool ok = false;
		TextureType type;
		type = DIFFUSE;
		if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS)
		{
			ok = true;
		}
		else if (material->GetTexture(aiTextureType_SPECULAR, texIndex, &path) == AI_SUCCESS)
		{
			ok = true;
		}
	
		if(ok)
		{
			std::string dir = GetDirectoryPath(filePath);
			std::string textureName = path.data;
			std::string fullPath = dir + textureName;
			int iTexFound = -1;
			for(int j = 0; j < m_textures.size(); j++)
			{
				if (fullPath == m_textures[j].texture.GetPath())
				{
					iTexFound = j;
					break;
				}
			}
			if (iTexFound != -1)
			{
				materialRemap[i] = iTexFound;
			}
			else
			{
				Texture tNew;
				tNew.LoadTexture2D(fullPath, true);
				tNew.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_TRILINEAR);
				materialRemap[i] = m_textures.size();
				TextureData texData;
				texData.texture = tNew;
				texData.type = type;
				m_textures.push_back(texData);
			}
		}
	}

	for (int i = 0; i < m_meshSizes.size(); i++)
	{
		int iOldIndex = m_materialIndices[i];
		m_materialIndices[i] = materialRemap[iOldIndex];
	}

	return m_bIsLoaded = true;
}

bool AssimpModel::LoadModelFromFileBumpMap(char* filePath, char* colorMap, char* normalMap)
{
	return true;
}

void AssimpModel::FinalizeVBO()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	m_vboModelData.BindVBO();
	m_vboModelData.UploadDataToGPU(GL_STATIC_DRAW);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(aiVector3D) + sizeof(aiVector2D) + sizeof(aiColor4D), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(aiVector3D) + sizeof(aiVector2D) + sizeof(aiColor4D), (void*)sizeof(aiVector3D));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(aiVector3D) + sizeof(aiVector2D) + sizeof(aiColor4D), (void*)(sizeof(aiVector3D) + sizeof(aiVector2D)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(aiVector3D) + sizeof(aiVector2D) + sizeof(aiColor4D), (void*)(sizeof(aiVector3D) + sizeof(aiVector2D) + sizeof(aiVector3D)));

}

void AssimpModel::BindModelsVAO()
{
	glBindVertexArray(m_VAO);
}

void AssimpModel::RenderModel()
{
	if (!m_bIsLoaded)
	{
		return;
	}
	int numMeshes = m_meshSizes.size();
	for (int i = 0; i < numMeshes; i++)
	{
		int iMatIndex = m_materialIndices[i];
		if (iMatIndex < m_textures.size())
		{
			Texture tex = m_textures[iMatIndex].texture;
			TextureType type = m_textures[iMatIndex].type;
			tex.BindTexture(type);
		}
		glDrawArrays(GL_TRIANGLES, m_meshStartIndices[i], m_meshSizes[i]);
	}
}
void AssimpModel::RenderModelBumpMap(ShaderProgram* program)
{

}

AssimpModel::AssimpModel()
{
	m_bIsLoaded = false;
}



