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
	std::vector<unsigned short> m_indeces;
public:

	void setVertexStream( size_t countOfFloat, const float* stream );
	void setTexCoordStream( size_t countOfFloat, const float* stream );
	void setIndexStream( size_t countOfShort, unsigned short* stream );

	void render();

	void onAdded();
	void onRemoved();
};
#endif // SWMesh_h__