#include "SWMesh.h"
#include "TVector3f.h"
#include "TVector2f.h"
#include "SWTriangle.h"
#include "SWGameContext.h"

void SWMesh::setVertexStream( size_t count, const TVector3f* stream )
{
	size_t newCount = count*3;
	size_t byteSize = sizeof(TVector3f)*count;
	m_vertices.resize( newCount );
	memcpy( &m_vertices[0], stream, byteSize );
	m_updateMesh = true;
}

void SWMesh::setTexCoordStream( size_t count, const TVector2f* stream )
{
	size_t streamSize = sizeof(TVector2f)*count;
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

void SWMesh::getVertexStream( tarray<TVector3f>& stream ) const
{
	stream = m_vertices;
}

void SWMesh::getTexCoordStream( tarray<TVector2f>& stream ) const
{
	stream = m_texCoords;
}

void SWMesh::getTriangleStream( tarray<SWTriangle>& stream ) const
{
	stream = m_triangles;
}

void SWMesh::setVertex( tuint index, const TVector3f& val )
{
	if ( index < m_vertices.size() ) m_vertices[index] = val;
}
void SWMesh::setTexCoord( tuint index, const TVector2f& val )
{
	if ( index < m_texCoords.size() ) m_texCoords[index] = val;
}
void SWMesh::setTriangle( tuint index, const SWTriangle& val )
{
	if ( index < m_triangles.size() ) m_triangles[index] = val;
}

void SWMesh::getVertex( tuint index, TVector3f& val )
{
	if ( index < m_vertices.size() ) val = m_vertices[index];
}
void SWMesh::getTexCoord( tuint index, TVector2f& val )
{
	if ( index < m_texCoords.size() ) val = m_texCoords[index];
}
void SWMesh::getTriangle( tuint index, SWTriangle& val )
{
	if ( index < m_triangles.size() ) val = m_triangles[index];
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
