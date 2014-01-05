#ifndef SWMesh_h__
#define SWMesh_h__

#include "SWObject.h"

class SWVector3f;
class SWVector2f;
class SWTriangle;
class SWMesh : public SWObject
{
	SW_RTTI( SWMesh, SWObject );

	tarray<SWVector3f>  m_vertices;
	tarray<SWVector2f>  m_texCoords;
	tarray<SWTriangle>  m_triangles;
	bool m_updateMesh;
public:

	void setVertexStream( size_t count, const SWVector3f* stream );
	void setTexCoordStream( size_t count, const SWVector2f* stream );
	void setTriangleStream( size_t count, const SWTriangle* stream );

	void resizeVertexStream( tuint count );
	void resizeTexCoordStream( tuint count );
	void resizeTriangleStream( tuint count );

	tuint getVertexCount() const;
	tuint getTexCoordCount() const;
	tuint getTriangleCount() const;

	const tarray<SWVector3f>& getVertexStream();
	const tarray<SWVector2f>& getTexCoordStream();
	const tarray<SWTriangle>& getTriangleStream();

	void updateMesh();
	void draw();
};

#endif // SWMesh_h__