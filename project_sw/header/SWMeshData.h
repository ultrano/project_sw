#ifndef SWMeshData_h__
#define SWMeshData_h__

#include "SWObject.h"
#include <vector>

class SWMeshData : public SWObject
{
	SW_RTTI( SWMeshData, SWObject );


	std::vector<float>  m_vertices;
	std::vector<float>  m_texCoords;
	std::vector<unsigned short> m_indeces;
public:

	void setVertexStream( size_t countOfFloat, const float* stream );
	void setTexCoordStream( size_t countOfFloat, const float* stream );
	void setIndexStream( size_t countOfShort, unsigned short* stream );

	std::vector<float>& getVertexStream();
	std::vector<float>& getTexCoordStream();
	std::vector<unsigned short>& getIndexStream();
};

#endif // SWMeshData_h__