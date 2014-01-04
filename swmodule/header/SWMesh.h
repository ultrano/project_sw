#ifndef SWMesh_h__
#define SWMesh_h__

#include "SWObject.h"

class SWVector3f;
class SWVector2f;
class SWMesh : public SWObject
{
	SW_RTTI( SWMesh, SWObject );

	tarray<SWVector3f>  m_vertices;
	tarray<SWVector2f>  m_texCoords;
	tarray<unsigned short> m_indeces;
	bool m_updateMesh;
public:

	void setVertexStream( size_t count, const SWVector3f* stream );
	void setTexCoordStream( size_t count, const SWVector2f* stream );
	void setIndexStream( size_t count, unsigned short* stream );

	const tarray<SWVector3f>& getVertexStream();
	const tarray<SWVector2f>& getTexCoordStream();
	const tarray<unsigned short>& getIndexStream();

	void updateMesh();
	void draw();
};

#endif // SWMesh_h__