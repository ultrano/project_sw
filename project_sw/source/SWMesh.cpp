#include "SWMesh.h"
#include "SWVector3f.h"
#include "SWVector2f.h"

void SWMesh::setVertexStream( size_t count, const SWVector3f* stream )
{
	unsigned int streamSize = sizeof(SWVector3f)*count;
	m_vertices.resize(count*3);
	memcpy( &m_vertices[0], stream, streamSize );
}

void SWMesh::setTexCoordStream( size_t count, const SWVector2f* stream )
{
	unsigned int streamSize = sizeof(SWVector2f)*count;
	m_texCoords.resize(count*2);
	memcpy( &m_texCoords[0], stream, streamSize );
}

void SWMesh::setIndexStream( size_t countOfShort, unsigned short* stream )
{
	m_indeces.resize(countOfShort);
	memcpy( &m_indeces[0], stream, sizeof(unsigned short)*countOfShort );
}

std::vector<float>& SWMesh::getTexCoordStream()
{
	return m_texCoords;
}

std::vector<unsigned short>& SWMesh::getIndexStream()
{
	return m_indeces;
}

std::vector<float>& SWMesh::getVertexStream()
{
	return m_vertices;
}

void SWMesh::updateMesh()
{
	
}
