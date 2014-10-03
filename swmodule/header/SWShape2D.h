#ifndef SWShape2D_h__
#define SWShape2D_h__

#include "SWType.h"
#include "SWRefCounter.h"
#include "SWRtti.h"

class SWShape2D : public SWRefCountable
{
	SW_RTTI_ROOT( SWShape2D );

public:

	virtual tvec2 getFarthest( const tvec2& direction ) = 0;

};

class SWCircle : public SWShape2D
{
	SW_RTTI( SWCircle, SWShape2D );
	
	tvec2 m_center;
	float m_radius;

	virtual tvec2 getFarthest( const tvec2& direction );
};

class SWPolygonShape2D : public SWShape2D
{
	SW_RTTI( SWPolygonShape2D, SWShape2D );
public:

	void set( const tarray<tvec2>& vertices );

private:
	tvec2 m_centroid;
	tarray<tvec2> m_vertices;
	tarray<tvec2> m_normals;
};


#endif //! SWShape2D_h__
