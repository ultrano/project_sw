#include "SWShape2D.h"
#include "SWMath.h"

SWCircle::SWCircle()
{

}

SWCircle::~SWCircle()
{

}

bool SWCircle::getFarthest( tvec2& farthest, const tvec2& direction, const Transform& transform  )
{
	float a = m_radius * transform.scale.x;
	float b = m_radius * transform.scale.y;

	tvec2 rot1( SWMath.cos(transform.rotate), SWMath.sin(transform.rotate) );
	tvec2 rot2 = rot1.cross( -1 );

	float t = SWMath.atan(direction.y, direction.x) - transform.rotate;
	float cost = SWMath.cos(t);
	float sint = SWMath.sin(t);
	tvec2 local( a*cost, b*sint );

	farthest.x = (local.x*rot1.x) + (local.y*rot2.x) + transform.move.x;
	farthest.y = (local.x*rot1.y) + (local.y*rot2.y) + transform.move.y;

	return false;
}

void SWCircle::computeAABB( taabb2d& aabb, const Transform& transform )
{
	aabb.lower = tvec2( -FLT_MAX, -FLT_MAX );
	aabb.upper = tvec2( FLT_MAX, FLT_MAX );
	float a = m_radius * transform.scale.x;
	float b = m_radius * transform.scale.y;

	tvec2 rot1( SWMath.cos(transform.rotate), SWMath.sin(transform.rotate) );
	tvec2 rot2 = rot1.cross( -1 );

	const tuint count = 4;
	tvec2 dirs[count] = { tvec2::axisX, tvec2::axisY, -tvec2::axisX, -tvec2::axisY };
	for ( tuint i = 0 ; i < count ; ++i )
	{
		const tvec2& direction = dirs[i];
		float t = SWMath.atan(direction.y, direction.x) - transform.rotate;
		float cost = SWMath.cos(t);
		float sint = SWMath.sin(t);
		tvec2 local( a*cost, b*sint );

		tvec2 point;
		point.x = (local.x*rot1.x) + (local.y*rot2.x) + transform.move.x;
		point.y = (local.x*rot1.y) + (local.y*rot2.y) + transform.move.y;
		aabb.min( point );
		aabb.max( point );
	}
}

SWPolygonShape2D::SWPolygonShape2D()
{

}

SWPolygonShape2D::~SWPolygonShape2D()
{

}

void SWPolygonShape2D::set( const tarray<tvec2>& vertices )
{
	if ( vertices.size() <= 2 ) return;

	m_normals.resize( vertices.size() );
	m_vertices.resize( vertices.size() );

	for ( tuint i = 0 ; i < vertices.size() ; ++i )
	{
		tuint i1 = i;
		tuint i2 = ((i+1) == vertices.size())? 0 : (i+1);
		
		const tvec2& v1 = vertices.at( i1 );
		const tvec2& v2 = vertices.at( i2 );
		tvec2 edge = v2 - v1;

		m_vertices[i] = v1;
		m_normals[i]  = edge.cross( 1 ).normal();
	}
	computeLocalOBB( m_localOBB );
}

void SWPolygonShape2D::setBox( float halfWidth, float halfHeight )
{
	tarray<tvec2> vertices;
	vertices.resize(4);
	vertices[0] = tvec2( halfWidth,  halfHeight);
	vertices[1] = tvec2(-halfWidth,  halfHeight);
	vertices[2] = tvec2( halfWidth, -halfHeight);
	vertices[3] = tvec2(-halfWidth, -halfHeight);
	set( vertices );
}

bool SWPolygonShape2D::getFarthest( tvec2& farthest, const tvec2& direction, const Transform& transform )
{
	if ( m_vertices.size() == 0 ) return false;

	tvec2 rot1( SWMath.cos( transform.rotate ), SWMath.sin( transform.rotate ) );
	tvec2 rot2 = rot1.cross( -1 );
	tvec3 col1(rot1.x * transform.scale.x, rot2.x * transform.scale.y, transform.move.x);
	tvec3 col2(rot1.y * transform.scale.x, rot2.y * transform.scale.y, transform.move.y);

	farthest = m_vertices.at(0);
	tvec2 v1;
	v1.x = (farthest.x * col1.x) + (farthest.y * col1.y) + col1.z;
	v1.y = (farthest.x * col2.x) + (farthest.y * col2.y) + col2.z;
	farthest = v1;
	float maxDist = direction.dot( v1 );

	tuint index = m_vertices.size();
	while ( index-- )
	{
		const tvec2& v = m_vertices.at( index );
		
		v1.x = (v.x * col1.x) + (v.y * col1.y) + col1.z;
		v1.y = (v.x * col2.x) + (v.y * col2.y) + col2.z;

		float dist = direction.dot( v1 );
		if ( dist > maxDist )
		{
			farthest = v1;
			maxDist = dist;
		}
	}

	return true;
}

void SWPolygonShape2D::computeAABB( taabb2d& aabb, const Transform& transform )
{
	tvec2 rot1( SWMath.cos( transform.rotate ), SWMath.sin( transform.rotate ) );
	tvec2 rot2 = rot1.cross( 1 );
	tvec3 col1(rot1.x * transform.scale.x, rot2.x * transform.scale.y, transform.move.x);
	tvec3 col2(rot1.y * transform.scale.x, rot2.y * transform.scale.y, transform.move.y);

	aabb.set( m_vertices[0], m_vertices[0] );
	tuint index = m_vertices.size();
	while ( index-- )
	{
		const tvec2& v = m_vertices.at( index );

		tvec2 v1;
		v1.x = (v.x * col1.x) + (v.y * col1.y) + col1.z;
		v1.y = (v.x * col2.x) + (v.y * col2.y) + col2.z;

		aabb.min( v1 );
		aabb.max( v1 );
	}
}

void SWPolygonShape2D::computeLocalOBB( tobb2d& obb )
{
	tpair<tvec2,tvec2> line1, line2;
	float minArea = FLT_MAX;

	//! find smallest area OBB according to the normals and cross-normals
	for ( tuint i = 0 ; i < m_normals.size() ; ++i)
	{
		const tvec2& n1 = m_normals[i];
		tvec2 n2( n1.y, -n1.x );

		tvec2 farthest1 = m_vertices[0];
		tvec2 nearest1  = farthest1;
		tvec2 farthest2 = farthest1;
		tvec2 nearest2  = farthest1;

		float dist1 = n1.dot( farthest1 );
		float dist2 = n2.dot( farthest2 );

		//! find farthest point
		for ( tuint j = 0 ; j < m_vertices.size() ; ++j )
		{
			float dist = 0;
			const tvec2& v = m_vertices[j];
			
			dist = n1.dot( v );
			if ( dist > dist1 )
			{
				farthest1 = v;
				dist1 = dist;
			}

			dist = n2.dot( v );
			if ( dist > dist2 )
			{
				farthest2 = v;
				dist2 = dist;
			}
		}
		
		dist1 = n1.dot( nearest1 );
		dist2 = n2.dot( nearest2 );

		//! find nearest point
		for ( tuint j = 0 ; j < m_vertices.size() ; ++j )
		{
			float dist = 0;
			const tvec2& v = m_vertices[j];

			dist = n1.dot( v );
			if ( dist < dist1 )
			{
				nearest1 = v;
				dist1 = dist;
			}

			dist = n2.dot( v );
			if ( dist < dist2 )
			{
				nearest2 = v;
				dist2 = dist;
			}
		}

		//! make the farthest and nearest points lied on the each normals
		{
			tvec2 middle;
			float length = 0;

			middle = (farthest1 + nearest1) * 0.5f;
			length = (farthest1 - middle).dot( n1 );
			farthest1 = middle + (length * n1);
			nearest1  = middle + (length * -n1);

			middle = (farthest2 + nearest2) * 0.5f;
			length = (farthest2 - middle).dot( n2 );
			farthest2 = middle + (length * n2);
			nearest2  = middle + (length * -n2);
		}

		//! switch to smallest OBB, if it is.
		float width  = (farthest1 - nearest1).length();
		float height = (farthest2 - nearest2).length();
		float area = width * height;
		if ( area < minArea )
		{
			minArea = area;
			tvec2 middle1 = (farthest1 + nearest1) * 0.5f;
			tvec2 middle2 = (farthest2 + nearest2) * 0.5f;
			tvec2 term = middle1 - middle2;
			obb.center = middle2 + (n1 * n1.dot(term));
			obb.halfSize.x = width * 0.5f;
			obb.halfSize.y = height * 0.5f;
			obb.radian = SWMath.atan( n1.y, n1.x );
		}
	}
}


float calculateArea( const tvec2& a, const tvec2& b, const tvec2& c )
{
	float area = 0;
	area += (a.x * b.y) + (b.x * c.y) + (c.x * a.y);
	area -= (a.y * b.x) + (b.y * c.x) + (c.y * a.x);
	if ( area < 0 ) area = -area;
	return area/2.0f;
}

bool testCollide
	( SWShape2D* shape1, const SWShape2D::Transform& transform1
	, SWShape2D* shape2, const SWShape2D::Transform& transform2 )
{
	tvec2 simplex[3];

	//! find first simplex
	{
		tvec2 dir, farthest1, farthest2;

		dir = tvec2::axisX;
		shape1->getFarthest( farthest1, dir, transform1 );
		shape2->getFarthest( farthest2, -dir, transform2 );
		simplex[0] = farthest2 - farthest1;

		dir = (-simplex[0]).normal();
		shape1->getFarthest( farthest1, dir, transform1 );
		shape2->getFarthest( farthest2, -dir, transform2 );
		simplex[1] = farthest2 - farthest1;

		tvec2 line = (simplex[1] - simplex[0]);
		float kz = line.cross( -simplex[0] );
		if ( kz == 0 )
		{
			if (  line.dot( -simplex[0] ) < 0 ) return false;
			if ( -line.dot( -simplex[1] ) < 0 ) return false;
			return true;
		}
		dir = line.cross( kz ).normal();
		shape1->getFarthest( farthest1, dir, transform1 );
		shape2->getFarthest( farthest2, -dir, transform2 );
		simplex[2] = farthest2 - farthest1;
	}

	//! try a few times to find overriding using Minkowski Difference and GJK
	tuint trying = 32;
	while ( trying-- )
	{
		float simplesArea = calculateArea(simplex[0], simplex[1], simplex[2]);
		float originArea = 0;
		originArea += calculateArea( tvec2::zero, simplex[0], simplex[1]);
		originArea += calculateArea( tvec2::zero, simplex[1], simplex[2]);
		originArea += calculateArea( tvec2::zero, simplex[2], simplex[0]);
		
		if ( simplesArea == originArea ) return true;

		tvec2 edge1 = simplex[1] - simplex[0];
		tvec2 edge2 = simplex[2] - simplex[1];
		tvec2 edge3 = simplex[0] - simplex[2];

		tvec2 dir1  = tvec2::zero - simplex[0];
		tvec2 dir2  = tvec2::zero - simplex[1];
		tvec2 dir3  = tvec2::zero - simplex[2];

		float kz1 = edge1.cross( dir1 );
		float kz2 = edge2.cross( dir2 );
		float kz3 = edge3.cross( dir3 );
		tuint count = 0;
		tuint index = 0;
		tvec2 edge;

		if ( kz1 > 0 ) {edge = edge1; index = 0; count += 1;}
		if ( kz2 > 0 ) {edge = edge2; index = 1; count += 1;}
		if ( kz3 > 0 ) {edge = edge3; index = 2; count += 1;}
		if ( count >= 2 ) return false;

		float kz = edge.cross(-simplex[index]);
		tvec2 dir = edge.cross( kz );
		tvec2 farthest1, farthest2;

		shape1->getFarthest( farthest1, dir, transform1 );
		shape2->getFarthest( farthest2, -dir, transform2 );
		simplex[(index+2)%3] = farthest2 - farthest1;
	}
	return false;
}