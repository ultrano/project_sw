#ifndef SWShape2D_h__
#define SWShape2D_h__

#include "SWType.h"
#include "SWRefCounter.h"
#include "SWRtti.h"

struct SWShapeTransform2D
{
	tvec2 scale;
	float rotate;
	tvec2 move;
	SWShapeTransform2D() : scale(tvec2::one), rotate(0), move(tvec2::zero) {}
};

class SWShape2D : public SWRefCountable
{
	SW_RTTI_ROOT( SWShape2D );

public:

	abstract bool getFarthest( tvec2& farthest, const tvec2& direction, const SWShapeTransform2D& transform ) const = 0;
	abstract void computeAABB( taabb2d& aabb, const SWShapeTransform2D& transform ) const = 0;
};

class SWCircleShape2D : public SWShape2D
{
	SW_RTTI( SWCircleShape2D, SWShape2D );
	
public:
	
	SWCircleShape2D();
	~SWCircleShape2D();

	void set( const tvec2& center, float radius ) { m_center = center; m_radius = radius; }

	void setCenter( const tvec2& center ) { m_center = center; }
	const tvec2& getCenter() const { return m_center; }

	void setRadius( float radius ) { m_radius = radius; }
	float getRadius() const { return m_radius; }

	override bool getFarthest( tvec2& farthest, const tvec2& direction, const SWShapeTransform2D& transform ) const;
	override void computeAABB( taabb2d& aabb, const SWShapeTransform2D& transform ) const;

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
	void setBox( const tvec2& center, float width, float height );
	override bool getFarthest( tvec2& farthest, const tvec2& direction, const SWShapeTransform2D& transform ) const;
	override void computeAABB( taabb2d& aabb, const SWShapeTransform2D& transform ) const;
private:
	void computeLocalOBB( tobb2d& obb );
private:
	tarray<tvec2> m_vertices;
	tarray<tvec2> m_normals;
	tobb2d m_localOBB;

};

class SWManifold : public SWMemory
{
public:
	tvec2 normal;
	float depth;
};

bool testShape2D( SWManifold& manifold, const SWShape2D* shape1, const SWShapeTransform2D& transform1 , const SWShape2D* shape2, const SWShapeTransform2D& transform2 );

#endif //! SWShape2D_h__
