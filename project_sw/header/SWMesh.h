#ifndef SWMesh_h__
#define SWMesh_h__

#include "SWObject.h"
#include <vector>

class SWVector3f;
class SWVector2f;
class SWMesh : public SWObject
{
	SW_RTTI( SWMesh, SWObject );

	std::vector<float>  m_vertices;
	std::vector<float>  m_texCoords;
	std::vector<unsigned short> m_indeces;
public:

	void setVertexStream( size_t count, const SWVector3f* stream );
	void setTexCoordStream( size_t count, const SWVector2f* stream );
	void setIndexStream( size_t count, unsigned short* stream );

	void updateMesh();

	std::vector<float>& getVertexStream();
	std::vector<float>& getTexCoordStream();
	std::vector<unsigned short>& getIndexStream();
};

#endif // SWMesh_h__