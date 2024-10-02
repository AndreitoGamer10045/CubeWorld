#pragma once

#include "glad/glad.h"

namespace vertexAttributes {
	void add(unsigned int location, int vertexAttributeSize, int dataType, bool isNormalized, int vertexByteSize, unsigned int vertexAttributeOffset);
};

class Buffer
{
public:
	Buffer(int type);
	~Buffer();

	void bind();
	void setData(unsigned int dataSize, const void* data, int drawingType);
	void updateData(unsigned int dataSize, const void* data);

	unsigned int getID();
	int getType();

private:
	unsigned int m_id;
	int m_type;
};

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void bind();

	unsigned int getID();

private:
	unsigned int m_id;
};