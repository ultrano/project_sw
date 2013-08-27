#include "SWMesh.h"
#include <memory>

void SWMesh::setVertexStream( const float* stream, size_t countOfFloat )
{
	m_vertices.resize(countOfFloat);
	memcpy( &m_vertices[0], stream, sizeof(float)*countOfFloat );
}

