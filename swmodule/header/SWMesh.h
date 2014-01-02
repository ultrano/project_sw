#ifndef SWMesh_h__
#define SWMesh_h__

#include "SWObject.h"

class SWVector3f;
class SWVector2f;
class SWMesh : public SWObject
{
	SW_RTTI( SWMesh, SWObject );

	tarray<float>  m_vertices;
	tarray<float>  m_texCoords;
	tarray<unsigned short> m_indeces;
	bool m_updateMesh;
public:

	void setVertexStream( size_t count, const SWVector3f* stream );
	void setTexCoordStream( size_t count, const SWVector2f* stream );
	void setIndexStream( size_t count, unsigned short* stream );

	tarray<float>& getVertexStream();
	tarray<float>& getTexCoordStream();
	tarray<unsigned short>& getIndexStream();

	void updateMesh();
	void draw();
};

#endif // SWMesh_h__