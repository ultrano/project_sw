#ifndef SWCollider2D_h__
#define SWCollider2D_h__

#include "SWBehavior.h"

class SWCollider2D : public SWBehavior
{
	SW_RTTI( SWCollider2D, SWBehavior );
public:
	
	virtual bool containPoint( const tvec2 point ) = 0;
};

class SWCircleCollider2D : public SWCollider2D
{
	SW_RTTI( SWCircleCollider2D, SWCollider2D );
private:

	float m_radius;
	tvec2 m_center;

public:

	void setRadius( float radius ) { m_radius = radius; };
	void setCenter( const tvec2& center ) { m_center = center; };

	float getRadius() const { return m_radius; };
	const tvec2& getCenter() const { return m_center; };
	
	bool containPoint( const tvec2 point );
};

#endif // SWCollider2D_h__