#ifndef SWMesh_h__
#define SWMesh_h__

#include "SWObject.h"
#include <vector>

class SWMesh : public SWObject
{
	SW_RTTI( SWMesh, SWObject );


	std::vector<float>  m_vertices;
	std::vector<float>  m_texCoords;
	std::vector<unsigned short> m_indeces;
public:

	void setVertexStream( size_t countOfFloat, const float* stream );
	void setTexCoordStream( size_t countOfFloat, const float* stream );
	void setIndexStream( size_t countOfShort, unsigned short* stream );

	void updateMesh();

	std::vector<float>& getVertexStream();
	std::vector<float>& getTexCoordStream();
	std::vector<unsigned short>& getIndexStream();
};

#endif // SWMesh_h__