#include "Shader.h"

#include <fstream>

#include <vector>
// GLEW Include
#include <GL\glew.h>

Shader::Shader()
{
}

bool Shader::LoadShader(std::string fileName, int shaderType)
{
	FILE* fp = fopen(fileName.c_str(), "r");
	if (!fp)
	{
		return false;
	}

	// Get Lines from file

	std::vector<std::string> lines;

	char line[255];

	while (fgets(line, 255, fp))
	{
		lines.push_back(line);
	}

	fclose(fp);
	const char** sProgram = new const char*[lines.size()];

	for (int i = 0; i < lines.size(); i++)
	{
		sProgram[i] = lines[i].c_str();
	}

	m_shaderID = glCreateShader(shaderType);

	glShaderSource(m_shaderID, lines.size(), sProgram, NULL);
	glCompileShader(m_shaderID);

	delete[] sProgram;

	GLchar infoLog[512];
	GLint iCompilationStatus;
	glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &iCompilationStatus);

	if (iCompilationStatus == GL_FALSE)
	{
		glGetShaderInfoLog(m_shaderID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	m_shaderType = shaderType;
	bIsLoaded = true;

	return true;
}

void Shader::DeleteShader()
{
	if (!IsLoaded())
	{
		return;
	}
	bIsLoaded = false;
	glDeleteShader(m_shaderID);
}

bool Shader::IsLoaded()
{
	return bIsLoaded;
}

unsigned int Shader::GetShaderID()
{
	return m_shaderID;
}

// Shader Program stuff

void ShaderProgram::CreateProgram()
{
	m_programID = glCreateProgram();
}
void ShaderProgram::DeleteProgram()
{
	if (bIsLinked)
	{
		return;
	}

	bIsLinked = false;
	glDeleteProgram(m_programID);
}

bool ShaderProgram::AddShaderToProgram(Shader* shader)
{
	if (!shader->IsLoaded())
	{
		return false;
	}

	glAttachShader(m_programID, shader->GetShaderID());

	return true;
}
bool ShaderProgram::LinkProgram()
{
	glLinkProgram(m_programID);
	int linkStatus;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &linkStatus);
	
	bIsLinked = (linkStatus == GL_TRUE);

	return bIsLinked;
}

void ShaderProgram::UseProgram()
{
	if (bIsLinked)
	{
		glUseProgram(m_programID);
	}	
}

unsigned int ShaderProgram::GetProgramID()
{
	return m_programID;
}


// Setting vectors
void ShaderProgram::SetUniform(std::string sName, glm::vec2* vVectors, int iCount)
{
	int loc = glGetUniformLocation(m_programID, sName.c_str());
	glUniform2fv(loc, iCount, (GLfloat*)vVectors);
}

void ShaderProgram::SetUniform(std::string sName, const glm::vec2 vVector)
{
	int loc = glGetUniformLocation(m_programID, sName.c_str());
	glUniform2fv(loc, 1, (GLfloat*)&vVector);

}
void ShaderProgram::SetUniform(std::string sName, glm::vec3* vVectors, int iCount)
{
	int loc = glGetUniformLocation(m_programID, sName.c_str());
	glUniform2fv(loc, iCount, (GLfloat*)vVectors);

}
void ShaderProgram::SetUniform(std::string sName, const glm::vec3 vVector)
{
	int loc = glGetUniformLocation(m_programID, sName.c_str());
	glUniform3fv(loc, 1, (GLfloat*)&vVector);

}
void ShaderProgram::SetUniform(std::string sName, glm::vec4* vVectors, int iCount)
{
	int loc = glGetUniformLocation(m_programID, sName.c_str());
	glUniform3fv(loc, iCount, (GLfloat*)vVectors);

}
void ShaderProgram::SetUniform(std::string sName, const glm::vec4 vVector)
{
	int loc = glGetUniformLocation(m_programID, sName.c_str());
	glUniform4fv(loc, 1, (GLfloat*)&vVector);

}

// Setting floats
void ShaderProgram::SetUniform(std::string sName, float* fValues, int iCount)
{
	int loc = glGetUniformLocation(m_programID, sName.c_str());
	glUniform1fv(loc, iCount, fValues);

}
void ShaderProgram::SetUniform(std::string sName, const float fValue)
{
	int loc = glGetUniformLocation(m_programID, sName.c_str());
	glUniform1fv(loc, 1, &fValue);

}

// Setting 4x4 matrices
void ShaderProgram::SetUniform(std::string sName, glm::mat4* mMatrices, int iCount)
{
	int loc = glGetUniformLocation(m_programID, sName.c_str());
	glUniformMatrix4fv(loc, iCount, GL_FALSE, (GLfloat*)mMatrices);

}
void ShaderProgram::SetUniform(std::string sName, const glm::mat4 mMatrix)
{
	int loc = glGetUniformLocation(m_programID, sName.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, (GLfloat*)&mMatrix);

}

// Setting integers
void ShaderProgram::SetUniform(std::string sName, int* iValues, int iCount)
{
	int loc = glGetUniformLocation(m_programID, sName.c_str());
	glUniform1iv(loc, iCount, iValues);
}

void ShaderProgram::SetUniform(std::string sName, const int iValue)
{
	int loc = glGetUniformLocation(m_programID, sName.c_str());
	glUniform1iv(loc, 1, &iValue);
}


ShaderProgram::ShaderProgram()
{
	bIsLinked = false;
}