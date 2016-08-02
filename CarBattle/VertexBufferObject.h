#pragma once

#include "GL/glew.h"
#include <vector>

typedef unsigned char BYTE;

class VertexBufferObject
{
public:
	void CreateVBO(int size = 0);
	void ReleaseVBO();

	void* MapBufferToMemory(int usageHint);
	void* MapSubBufferToMemory(int usageHint, unsigned int offset, unsigned int length);
	void UnmapBuffer();

	void BindVBO(int bufferType = GL_ARRAY_BUFFER);
	void UploadDataToGPU(int usageHint);

	void AddData(void* ptrData, unsigned int dataSize);

	void* GetDataPointer();
	unsigned int GetBuffer();

	int GetCurrentSize();

	VertexBufferObject();

private:
	unsigned int m_buffer;
	int m_size;
	int m_bufferSize;
	int m_bufferType;
	std::vector<BYTE> m_data;

	bool m_bDataSent;

};

