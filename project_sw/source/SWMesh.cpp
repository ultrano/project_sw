#include "SWMesh.h"

void SWMesh::setVertexStream( size_t countOfFloat, const float* stream )
{
	m_vertices.resize(countOfFloat);
	memcpy( &m_vertices[0], stream, sizeof(float)*countOfFloat );
}

void SWMesh::setTexCoordStream( size_t countOfFloat, const float* stream )
{
	m_texCoords.resize(countOfFloat);
	memcpy( &m_texCoords[0], stream, sizeof(float)*countOfFloat );
}

std::vector<float>& SWMesh::getTexCoordStream()
{
	return m_texCoords;
}

std::vector<unsigned short>& SWMesh::getIndexStream()
{
	return m_indeces;
}

void SWMesh::setIndexStream( size_t countOfShort, unsigned short* stream )
{
	m_indeces.resize(countOfShort);
	memcpy( &m_indeces[0], stream, sizeof(unsigned short)*countOfShort );
}

std::vector<float>& SWMesh::getVertexStream()
{
	return m_vertices;
}

void SWMesh::updateMesh()
{
	
}
