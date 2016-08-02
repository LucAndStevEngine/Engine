#pragma once
#include <iostream>
#include "glm\glm.hpp"

class Shader
{
public:
	bool LoadShader(std::string fileName, int shaderType);
	void DeleteShader();

	bool IsLoaded();
	unsigned int GetShaderID();

	Shader();

private:
	unsigned int m_shaderID;
	int m_shaderType;
	bool bIsLoaded;
};


class ShaderProgram
{
public:
	void CreateProgram();
	void DeleteProgram();

	bool AddShaderToProgram(Shader* shader);
	bool LinkProgram();

	void UseProgram();

	unsigned int GetProgramID();

	// Setting vectors
	void SetUniform(std::string sName, glm::vec2* vVectors, int iCount = 1);
	void SetUniform(std::string sName, const glm::vec2 vVector);
	void SetUniform(std::string sName, glm::vec3* vVectors, int iCount = 1);
	void SetUniform(std::string sName, const glm::vec3 vVector);
	void SetUniform(std::string sName, glm::vec4* vVectors, int iCount = 1);
	void SetUniform(std::string sName, const glm::vec4 vVector);

	// Setting floats
	void SetUniform(std::string sName, float* fValues, int iCount = 1);
	void SetUniform(std::string sName, const float fValue);

	// Setting 4x4 matrices
	void SetUniform(std::string sName, glm::mat4* mMatrices, int iCount = 1);
	void SetUniform(std::string sName, const glm::mat4 mMatrix);

	// Setting integers
	void SetUniform(std::string sName, int* iValues, int iCount = 1);
	void SetUniform(std::string sName, const int iValue);

	ShaderProgram();

private:
	unsigned int m_programID;
	bool bIsLinked;
};
