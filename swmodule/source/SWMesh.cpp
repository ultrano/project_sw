#include "SWMesh.h"
#include "SWVector3f.h"
#include "SWVector2f.h"
#include "SWGameContext.h"

void SWMesh::setVertexStream( size_t count, const SWVector3f* stream )
{
	size_t newCount = count*3;
	size_t oldCount = m_vertices.size();
	size_t byteSize = sizeof(SWVector3f)*count;
	if ( newCount > oldCount ) m_vertices.resize( newCount );
	
	memcpy( &m_vertices[0], stream, byteSize );

	if ( oldCount > newCount )
	{
		size_t spareSize = sizeof(SWVector3f)*(oldCount - newCount);
		memset( &m_vertices[newCount], 0, spareSize );
	}

	m_updateMesh = true;
}

void SWMesh::setTexCoordStream( size_t count, const SWVector2f* stream )
{
	size_t streamSize = sizeof(SWVector2f)*count;
	m_texCoords.resize(count*2);
	memcpy( &m_texCoords[0], stream, streamSize );
	m_updateMesh = true;
}

void SWMesh::setIndexStream( size_t countOfShort, unsigned short* stream )
{
	m_indeces.resize(countOfShort);
	memcpy( &m_indeces[0], stream, sizeof(unsigned short)*countOfShort );
	m_updateMesh = true;
}

tarray<float>& SWMesh::getTexCoordStream()
{
	return m_texCoords;
}

tarray<unsigned short>& SWMesh::getIndexStream()
{
	return m_indeces;
}

tarray<float>& SWMesh::getVertexStream()
{
	return m_vertices;
}

void SWMesh::updateMesh()
{
	m_updateMesh = false;
	//! this is for when using graphic buffer
}

void SWMesh::draw()
{
	static int lastMeshID = 0;
	if ( getID() != lastMeshID )
	{
	  if ( m_updateMesh ) updateMesh();
	  SW_GC.setVertexBuffer( &m_vertices[0] );
	  SW_GC.setTexCoordBuffer( &m_texCoords[0] );
	}
	lastMeshID = getID();
	SW_GC.drawIndexed( m_indeces.size(), &m_indeces[0] );
}
