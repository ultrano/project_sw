#ifndef SWMesh_h__
#define SWMesh_h__

#include "SWObject.h"

class TVector3f;
class TVector2f;
class SWIndex3;
class SWMesh : public SWObject
{
	SW_RTTI( SWMesh, SWObject );

	tarray< TVector3f >  m_vertices;
	tarray< TVector2f >  m_texCoords;
	tarray< SWIndex3 >  m_triangles;
	bool m_updateMesh;
public:

	void setVertexStream( size_t count, const TVector3f* stream );
	void setTexCoordStream( size_t count, const TVector2f* stream );
	void setTriangleStream( size_t count, const SWIndex3* stream );

	void resizeVertexStream( tuint count );
	void resizeTexCoordStream( tuint count );
	void resizeTriangleStream( tuint count );

	tuint getVertexCount() const;
	tuint getTexCoordCount() const;
	tuint getTriangleCount() const;

	void setVertex( tuint index, const TVector3f& val );
	void setTexCoord( tuint index, const TVector2f& val );
	void setTriangle( tuint index, const SWIndex3& val );
	
	void getVertex( tuint index, TVector3f& val );
	void getTexCoord( tuint index, TVector2f& val );
	void getTriangle( tuint index, SWIndex3& val );

	void getVertexStream( tarray<TVector3f>& stream ) const;
	void getTexCoordStream( tarray<TVector2f>& stream ) const;
	void getTriangleStream( tarray<SWIndex3>& stream ) const;

	void updateMesh();
	void draw();
};

#endif // SWMesh_h__