#include "engine/Buffer.hpp"

void vertexAttributes::add(unsigned int location, int vertexAttributeSize, int dataType, bool isNormalized, int vertexByteSize, unsigned int vertexAttributeOffset)
{
	glVertexAttribPointer(location, vertexAttributeSize, dataType, isNormalized, vertexByteSize, (void*)vertexAttributeOffset);
	glEnableVertexAttribArray(location);
}


Buffer::Buffer(int type)
{
	glGenBuffers(1, &m_id);
	m_type = type;
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &m_id);
}


void Buffer::bind()
{
	glBindBuffer(m_type, m_id);
}

void Buffer::setData(unsigned int dataByteSize, const void* data, int drawingUsage)
{
	glBufferData(m_type, dataByteSize, data, drawingUsage);
}

void Buffer::updateData(unsigned int dataSize, const void* data)
{
	glBufferSubData(m_type, 0, dataSize, data);
}


unsigned int Buffer::getID()
{
	return m_id;
}

int Buffer::getType()
{
	return m_type;
}


VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_id);
}


void VertexArray::bind()
{
	glBindVertexArray(m_id);
}


unsigned int VertexArray::getID()
{
	return m_id;
}