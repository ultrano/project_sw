#include "SWMesh.h"
#include "SWGameContext.h"
#include <memory>

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

void SWMesh::setIndexStream( size_t countOfShort, unsigned short* stream )
{
	m_indeces.resize(countOfShort);
	memcpy( &m_indeces[0], stream, sizeof(unsigned short)*countOfShort );
}

void SWMesh::render()
{
	SW_GC.setVertexBuffer( &m_vertices[0] );
	SW_GC.setTexCoordBuffer( &m_texCoords[0] );
	SW_GC.indexedDraw( m_indeces.size(), &m_indeces[0] );
}

