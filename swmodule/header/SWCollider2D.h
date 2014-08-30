#ifndef SWCollider2D_h__
#define SWCollider2D_h__

#include "SWComponent.h"

class SWCollider2D : public SWComponent
{
	SW_RTTI( SWCollider2D, SWComponent );
public:

	void onAwake();
	void onRemove();
	
	virtual bool containPoint( const tvec2& worldPt ) = 0;
	virtual void farthestPoint( tvec2& ret, const tvec2& worldDir ) = 0;
};

class SWCollision2D : public SWObject
{
	SW_RTTI( SWCollision2D, SWObject );

public:

	SWCollision2D() : collider( NULL ) {};
	SWCollision2D( SWCollider2D* _collider ) : collider( _collider ) {};
	
	SWWeakRef<SWCollider2D> collider;
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
	
	float getWorldRadius() const;
	tvec2 getWorldCenter() const;

	bool containPoint( const tvec2& point );
	void farthestPoint( tvec2& ret, const tvec2& worldDir );

protected:

	void serialize( SWObjectWriter* ow );
	void deserialize( SWObjectReader* or );

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

	tvec2 getWorldCenter() const;
	void getWorldEdges( tvec2& edge1, tvec2& edge2, tvec2& edge3, tvec2& edge4 ) const;

	bool containPoint( const tvec2& point );
	void farthestPoint( tvec2& ret, const tvec2& worldDir );
	
protected:

	void getSREdges( tvec2& edge1, tvec2& edge2, tvec2& edge3, tvec2& edge4 ) const;

	void serialize( SWObjectWriter* ow );
	void deserialize( SWObjectReader* or );

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