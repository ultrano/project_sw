#ifndef SWMesh_h__
#define SWMesh_h__

#include "SWObject.h"

class TVector3f;
class TVector2f;
class TIndex3;
class SWMesh : public SWObject
{
	SW_RTTI( SWMesh, SWObject );

public:

    SWMesh();
	SWMesh( factory_constructor );
    ~SWMesh();
    
	void setVertexStream( size_t count, const TVector3f* stream );
	void setTexCoordStream( size_t count, const TVector2f* stream );
	void setTriangleStream( size_t count, const TIndex3* stream );

	void resizeVertexStream( tuint count );
	void resizeTexCoordStream( tuint count );
	void resizeTriangleStream( tuint count );

	tuint getVertexCount() const;
	tuint getTexCoordCount() const;
	tuint getTriangleCount() const;

	void setVertex( tuint index, const TVector3f& val );
	void setTexCoord( tuint index, const TVector2f& val );
	void setTriangle( tuint index, const TIndex3& val );
	
	void getVertex( tuint index, TVector3f& val );
	void getTexCoord( tuint index, TVector2f& val );
	void getTriangle( tuint index, TIndex3& val );

	void getVertexStream( tarray<TVector3f>& stream ) const;
	void getTexCoordStream( tarray<TVector2f>& stream ) const;
	void getTriangleStream( tarray<TIndex3>& stream ) const;

	void updateMesh();
	void draw();
    
protected:

	void serialize( SWObjectWriter* ow );
	void deserialize( SWObjectReader* or );

private:
	tarray< TVector3f >  m_vertices;
	tarray< TVector2f >  m_texCoords;
	tarray< TIndex3 >    m_triangles;
	bool m_updateMesh;
    tuint m_vaoID;
    tuint m_vboID;
    tuint m_iboID;
};

#endif // SWMesh_h__