#ifndef SWMesh_h__
#define SWMesh_h__

#include "SWComponent.h"
#include <vector>
class SWMesh : public SWComponent
{
	SW_RTTI( SWMesh, SWComponent );
private:

	std::vector<float>  m_vertices;
	std::vector<float>  m_texCoords;

public:

	void setVertexStream( const float* stream, size_t countOfFloat );

	void render(){};
};
#endif // SWMesh_h__