#include "VertexBufferObject.h"



VertexBufferObject::VertexBufferObject()
{
	m_bDataSent = false;
}

void VertexBufferObject::CreateVBO(int size)
{
	glGenBuffers(1, &m_buffer);
	m_data.reserve(size);
	m_bufferSize = size;
}

void VertexBufferObject::ReleaseVBO()
{
	glDeleteBuffers(1, &m_buffer);
	m_bDataSent = false;
	m_data.clear();
}

void* VertexBufferObject::MapBufferToMemory(int usageHint)
{
	if (!m_bDataSent)
	{
		return NULL;
	}
	
	void* ptrRes = glMapBuffer(m_bufferType, usageHint);
	return ptrRes;

}

void* VertexBufferObject::MapSubBufferToMemory(int usageHint, unsigned int offset, unsigned int length)
{
	if (!m_bDataSent)
	{
		return NULL;
	}

	void* ptrRes = glMapBufferRange(m_bufferType, offset, length, usageHint);
	return ptrRes;

}

void VertexBufferObject::UnmapBuffer()
{
	glUnmapBuffer(m_bufferType);
}

void VertexBufferObject::BindVBO(int bufferType)
{
	m_bufferType = bufferType;
	glBindBuffer(bufferType, m_buffer);
}

void VertexBufferObject::UploadDataToGPU(int drawingHint)
{
	glBufferData(m_bufferType, m_data.size(), &m_data[0], drawingHint);
}

void VertexBufferObject::AddData(void* ptrData, unsigned int dataSize)
{
	m_data.insert(m_data.end(), (BYTE*)ptrData, (BYTE*)ptrData + dataSize);
	m_size += dataSize;
}

void* VertexBufferObject::GetDataPointer()
{
	if (m_bDataSent)
	{
		return NULL;
	}

	return &m_data;
}

unsigned int VertexBufferObject::GetBuffer()
{
	return m_buffer;
}

int VertexBufferObject::GetCurrentSize()
{
	return m_size;
}