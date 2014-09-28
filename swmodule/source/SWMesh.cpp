#include "SWMesh.h"
#include "TVector3f.h"
#include "TVector2f.h"
#include "TIndex3.h"
#include "SWOpenGL.h"
#include "SWDefines.h"
#include "SWMath.h"
#include "SWObjectStream.h"

struct SWMeshVertex
{
    tvec3 pos;
    tvec2 tex;
};

SWMesh::SWMesh()
	: m_vboID( 0 )
	, m_iboID( 0 )
	, m_updateMesh( false )
{
    
}

SWMesh::SWMesh( factory_constructor )
	: m_vboID( 0 )
	, m_iboID( 0 )
	, m_updateMesh( false )
{

}

SWMesh::~SWMesh()
{
    if ( m_vboID != 0 ) glDeleteBuffers( 1, &m_vboID );
    if ( m_iboID != 0 ) glDeleteBuffers( 1, &m_iboID );
}

void SWMesh::setVertexStream( size_t count, const TVector3f* stream )
{
	m_vertices.resize( count );
	memcpy( &m_vertices[0], stream, sizeof(TVector3f)*count );
	m_updateMesh = true;
}

void SWMesh::setTexCoordStream( size_t count, const TVector2f* stream )
{
	m_texCoords.resize(count);
	memcpy( &m_texCoords[0], stream, sizeof(TVector2f)*count );
	m_updateMesh = true;
}

void SWMesh::setTriangleStream( size_t count, const TIndex3* stream )
{
	m_triangles.resize(count);
	memcpy( &m_triangles[0], stream, sizeof(TIndex3)*count );
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

void SWMesh::getTriangleStream( tarray<TIndex3>& stream ) const
{
	stream = m_triangles;
}

void SWMesh::setVertex( tuint index, const TVector3f& val )
{
	if ( index < m_vertices.size() ) m_vertices[index] = val;
	m_updateMesh = true;
}
void SWMesh::setTexCoord( tuint index, const TVector2f& val )
{
	if ( index < m_texCoords.size() ) m_texCoords[index] = val;
	m_updateMesh = true;
}
void SWMesh::setTriangle( tuint index, const TIndex3& val )
{
	if ( index < m_triangles.size() ) m_triangles[index] = val;
	m_updateMesh = true;
}

void SWMesh::getVertex( tuint index, TVector3f& val )
{
	if ( index < m_vertices.size() ) val = m_vertices[index];
}
void SWMesh::getTexCoord( tuint index, TVector2f& val )
{
	if ( index < m_texCoords.size() ) val = m_texCoords[index];
}
void SWMesh::getTriangle( tuint index, TIndex3& val )
{
	if ( index < m_triangles.size() ) val = m_triangles[index];
}

void SWMesh::updateMesh()
{
	m_updateMesh = false;

	//! refresh AABB
	tuint count = m_vertices.size();
	if ( count > 0)
	{
		tvec3 lower = m_vertices[0];
		tvec3 upper = lower;
		while ( count-- )
		{
			const tvec3& pos = m_vertices[count];
			lower.x = SWMath.min( lower.x, pos.x );
			lower.y = SWMath.min( lower.y, pos.y );
			lower.z = SWMath.min( lower.z, pos.z );
			upper.x = SWMath.max( upper.x, pos.x );
			upper.y = SWMath.max( upper.y, pos.y );
			upper.z = SWMath.max( upper.z, pos.z );
		}
		m_aabb.lower = lower;
		m_aabb.upper = upper;
	}

    /*
	//! this is for when using graphic buffer
    if ( m_vboID == 0 ) glGenBuffers( 1, &m_vboID );
    if ( m_iboID == 0 ) glGenBuffers( 1, &m_iboID );
    
    //! set vertex data
    {
        tuint count = SWMath.max<tuint>( m_vertices.size(), m_texCoords.size() );
        tarray<SWMeshVertex> vbo;
        for ( int i=0 ; i < count ; ++i )
        {
            SWMeshVertex vertex;
            vertex.pos = ( m_vertices.size() > i )? m_vertices[i] : tvec3::zero;
            vertex.tex = ( m_texCoords.size() > i )? m_texCoords[i] : tvec2::zero;
            vbo.push_back( vertex );
        }
        glBindBuffer(GL_ARRAY_BUFFER, m_vboID );
        glBufferData(GL_ARRAY_BUFFER, count*sizeof(SWMeshVertex), &vbo[0], GL_STATIC_DRAW );
    }
    
    //! set index data
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID );
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_triangles.size()*sizeof(TIndex3), &m_triangles[0], GL_STATIC_DRAW );
    }
    
    //! clear binding
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0 );
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );
    }
    */
}

void SWMesh::draw()
{
	static int lastMeshID = 0;
	if ( getID() != lastMeshID )
	{
        if ( m_updateMesh ) updateMesh();
        if ( m_vertices.size() == 0 ) return;
        
        glVertexAttribPointer( SW_Attribute_Position, 3, GL_FLOAT, GL_FALSE, 0, (float*)&m_vertices[0] );
        glVertexAttribPointer( SW_Attribute_Texture, 2, GL_FLOAT, GL_FALSE, 0, (float*)&m_texCoords[0] );
	}
	lastMeshID = getID();
	if ( m_triangles.size() == 0 ) return;
	glDrawElements( GL_TRIANGLES, m_triangles.size()*3, GL_UNSIGNED_SHORT, (tushort*)&m_triangles[0] );
    /*
	static int lastMeshID = 0;
	if ( getID() != lastMeshID )
	{
	  if ( m_updateMesh ) updateMesh();
	  if ( m_vertices.size() == 0 ) return;
        
        glBindBuffer(GL_ARRAY_BUFFER, m_vboID );
	 
        glVertexAttribPointer( SW_Attribute_Position
                            , 3
                            , GL_FLOAT, GL_FALSE
                            , sizeof( SWMeshVertex ), 0 );
	  
        glVertexAttribPointer( SW_Attribute_Texture
                            , 2
                            , GL_FLOAT, GL_FALSE
                            , sizeof( SWMeshVertex ), (void*)sizeof(tvec3) );
	}
	lastMeshID = getID();
	if ( m_triangles.size() == 0 ) return;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID );
	glDrawElements( GL_TRIANGLES, m_triangles.size()*3, GL_UNSIGNED_SHORT, 0 );
     */
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

void SWMesh::serialize( SWObjectWriter* writer )
{
	writer->writeUInt( m_vertices.size() );
	for ( tuint i = 0 ; i < m_vertices.size() ; ++i)
	{
		writer->writeVec3( m_vertices[i] );
	}
	writer->writeUInt( m_texCoords.size() );
	for ( tuint i = 0 ; i < m_texCoords.size() ; ++i)
	{
		writer->writeVec2( m_texCoords[i] );
	}
	writer->writeUInt( m_triangles.size() );
	for ( tuint i = 0 ; i < m_triangles.size() ; ++i)
	{
		writer->writeIndex3( m_triangles[i] );
	}
}

void SWMesh::deserialize( SWObjectReader* reader )
{
	m_vertices.resize( reader->readUInt() );
	for ( tuint i = 0 ; i < m_vertices.size() ; ++i)
	{
		reader->readVec3( m_vertices[i] );
	}
	m_texCoords.resize( reader->readUInt() );
	for ( tuint i = 0 ; i < m_texCoords.size() ; ++i)
	{
		reader->readVec2( m_texCoords[i] );
	}
	m_triangles.resize( reader->readUInt() );
	for ( tuint i = 0 ; i < m_triangles.size() ; ++i)
	{
		reader->readIndex3( m_triangles[i] );
	}
}

const taabb3d& SWMesh::getAABB() const
{
	return m_aabb;
}
