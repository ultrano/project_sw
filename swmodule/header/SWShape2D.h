#ifndef SWShape2D_h__
#define SWShape2D_h__

#include "SWType.h"
#include "SWRefCounter.h"
#include "SWRtti.h"

class SWShape2D : public SWRefCountable
{
	SW_RTTI_ROOT( SWShape2D );

public:

	struct Transform
	{
		tvec2 scale;
		float rotate;
		tvec2 move;
	};

	abstract bool getFarthest( tvec2& farthest, const tvec2& direction, const Transform& transform ) = 0;
	abstract void computeAABB( taabb2d& aabb, const Transform& transform ) = 0;
};

class SWCircle : public SWShape2D
{
	SW_RTTI( SWCircle, SWShape2D );
	
public:
	
	SWCircle();
	~SWCircle();

	void set( const tvec2& center, float radius ) { m_center = center; m_radius = radius; }

	void setCenter( const tvec2& center ) { m_center = center; }
	const tvec2& getCenter() const { return m_center; }

	void setRadius( float radius ) { m_radius = radius; }
	float getRadius() const { return m_radius; }

	override bool getFarthest( tvec2& farthest, const tvec2& direction, const Transform& transform );
	override void computeAABB( taabb2d& aabb, const Transform& transform );

private:
	tvec2 m_center;
	float m_radius;
};

class SWPolygonShape2D : public SWShape2D
{
	SW_RTTI( SWPolygonShape2D, SWShape2D );
public:

	SWPolygonShape2D();
	~SWPolygonShape2D();

	void set( const tarray<tvec2>& vertices );
	void setBox( float halfWidth, float halfHeight );
	override bool getFarthest( tvec2& farthest, const tvec2& direction, const Transform& transform );
	override void computeAABB( taabb2d& aabb, const Transform& transform );
private:
	void computeLocalOBB( tobb2d& obb );
private:
	tarray<tvec2> m_vertices;
	tarray<tvec2> m_normals;
	tobb2d m_localOBB;

};

bool testCollide( SWShape2D* shape1, const SWShape2D::Transform& transform1 , SWShape2D* shape2, const SWShape2D::Transform& transform2 );

#endif //! SWShape2D_h__
