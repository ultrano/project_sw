#include "SWMesh.h"
#include "SWVector3f.h"
#include "SWVector2f.h"
#include "SWTriangle.h"
#include "SWGameContext.h"

void SWMesh::setVertexStream( size_t count, const SWVector3f* stream )
{
	size_t newCount = count*3;
	size_t byteSize = sizeof(SWVector3f)*count;
	m_vertices.resize( newCount );
	memcpy( &m_vertices[0], stream, byteSize );
	m_updateMesh = true;
}

void SWMesh::setTexCoordStream( size_t count, const SWVector2f* stream )
{
	size_t streamSize = sizeof(SWVector2f)*count;
	m_texCoords.resize(count*2);
	memcpy( &m_texCoords[0], stream, streamSize );
	m_updateMesh = true;
}

void SWMesh::setTriangleStream( size_t count, const SWTriangle* stream )
{
	m_triangles.resize(count);
	memcpy( &m_triangles[0], stream, sizeof(SWTriangle)*count );
	m_updateMesh = true;
}

const tarray<SWVector2f>& SWMesh::getTexCoordStream()
{
	return m_texCoords;
}

const tarray<SWTriangle>& SWMesh::getTriangleStream()
{
	return m_triangles;
}

const tarray<SWVector3f>& SWMesh::getVertexStream()
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
	  if ( m_vertices.size() == 0 ) return;
	  SW_GC.setVertexBuffer( (float*)&m_vertices[0] );
	  SW_GC.setTexCoordBuffer( (float*)&m_texCoords[0] );
	}
	lastMeshID = getID();
	if ( m_triangles.size() == 0 ) return;
	SW_GC.drawIndexed( m_triangles.size()*3, (tushort*)&m_triangles[0] );
}

void SWMesh::resizeVertexStream( tuint count )
{
	m_vertices.resize( count );
}

void SWMesh::resizeTexCoordStream( tuint count )
{
	m_texCoords.resize( count );
}

void SWMesh::resizeTriangleStream( tuint count )
{
	m_triangles.resize( count );
}

tuint SWMesh::getVertexCount() const
{
	return m_vertices.size();
}

tuint SWMesh::getTexCoordCount() const
{
	return m_texCoords.size();
}

tuint SWMesh::getTriangleCount() const
{
	return m_triangles.size();
}
