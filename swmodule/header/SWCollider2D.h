#ifndef SWCollider2D_h__
#define SWCollider2D_h__

#include "SWBehavior.h"

class SWCollider2D : public SWBehavior
{
	SW_RTTI( SWCollider2D, SWBehavior );
public:

	void onAwake();
	void onRemove();
	
	virtual bool containPoint( const tvec2 point ) = 0;
};

class SWCircleCollider2D : public SWCollider2D
{
	SW_RTTI( SWCircleCollider2D, SWCollider2D );

public:

	SWCircleCollider2D( factory_constructor );
	~SWCircleCollider2D();

	void setRadius( float radius ) { m_radius = radius; };
	void setCenter( const tvec2& center ) { m_center = center; };

	float getRadius() const { return m_radius; };
	const tvec2& getCenter() const { return m_center; };
	
	bool containPoint( const tvec2 point );

private:

	float m_radius;
	tvec2 m_center;
};

class SWRectCollider2D : public SWCollider2D
{
	SW_RTTI( SWRectCollider2D, SWCollider2D );
public:
	
	SWRectCollider2D( factory_constructor );
	~SWRectCollider2D();

	void setSize( const tvec2& size ) { m_size = size; }
	void setCenter( const tvec2& center ) { m_center = center; }

	bool containPoint( const tvec2 point );

private:
	tvec2 m_size;
	tvec2 m_center;
};

/*
class SWPolygonCollider2D : public SWCollider2D
{
	SW_RTTI( SWPolygonCollider2D, SWCollider2D );
	
public:

	const tvec2& getVertex( tuint index );

private:

	tarray<tvec2> m_vertices;

}
*/
#endif // SWCollider2D_h__