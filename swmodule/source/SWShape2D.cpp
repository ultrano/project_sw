#include "SWShape2D.h"
#include "SWMath.h"
#include "SWLog.h"
#define SW_Epsilon (0.05f)

SWCircleShape2D::SWCircleShape2D()
{

}

SWCircleShape2D::~SWCircleShape2D()
{

}

bool SWCircleShape2D::getFarthest( tvec2& farthest, const tvec2& direction, const Transform& transform  ) const
{
	float a = m_radius * transform.scale.x;
	float b = m_radius * transform.scale.y;

	tvec2 rot1( SWMath.cos(transform.rotate), SWMath.sin(transform.rotate) );
	tvec2 rot2 = rot1.cross( -1 );

	float t = SWMath.atan(direction.y, direction.x) - transform.rotate;
	float cost = SWMath.cos(t);
	float sint = SWMath.sin(t);
	tvec2 local = tvec2( a*cost, b*sint ) + m_center;

	farthest.x = (local.x*rot1.x) + (local.y*rot2.x) + transform.move.x;
	farthest.y = (local.x*rot1.y) + (local.y*rot2.y) + transform.move.y;

	return false;
}

void SWCircleShape2D::computeAABB( taabb2d& aabb, const Transform& transform ) const
{
	aabb.lower = tvec2( FLT_MAX, FLT_MAX );
	aabb.upper = -aabb.lower;
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
		tvec2 local = tvec2( a*cost, b*sint ) + m_center;

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

void SWPolygonShape2D::setBox( const tvec2& center, float width, float height )
{
	float halfWidth  = width/2.0f;
	float halfHeight = height/2.0f;
	tarray<tvec2> vertices;
	vertices.resize(4);
	vertices[0] = center + tvec2( halfWidth,  halfHeight);
	vertices[1] = center + tvec2(-halfWidth,  halfHeight);
	vertices[2] = center + tvec2(-halfWidth, -halfHeight);
	vertices[3] = center + tvec2( halfWidth, -halfHeight);
	set( vertices );
}

bool SWPolygonShape2D::getFarthest( tvec2& farthest, const tvec2& direction, const Transform& transform ) const
{
	if ( m_vertices.size() == 0 ) return false;

	tmat33 mat;
	mat.set( transform.scale, transform.rotate, transform.move );

	farthest = m_vertices.at(0) * mat;
	float maxDist = direction.dot( farthest );

	tuint index = m_vertices.size();
	while ( index-- )
	{
		const tvec2& v = m_vertices.at( index );

		tvec2 v1 = v * mat;
		float dist = direction.dot( v1 );
		if ( dist > maxDist )
		{
			farthest = v1;
			maxDist = dist;
		}
	}

	return true;
}

void SWPolygonShape2D::computeAABB( taabb2d& aabb, const Transform& transform ) const
{
	tmat33 mat;
	mat.set( transform.scale, transform.rotate, transform.move );

	aabb.lower = tvec2(FLT_MAX,FLT_MAX);
	aabb.upper = -aabb.lower;
	tuint index = m_vertices.size();
	while ( index-- )
	{
		const tvec2& v = m_vertices[index];

		tvec2 v1 = v * mat;

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

void calculateSide( tflag8& sideFlag, const tvec2& a, const tvec2& b, const tvec2& c )
{
	tvec2 edgeA = b - a;
	tvec2 edgeB = c - b;
	tvec2 edgeC = a - c;
	tvec2 center = (a+b+c)/3.0f;
	float det(0), val(0);

	det = edgeA.cross(center - a);
	val = edgeA.cross(-a);
	sideFlag.set(0, (det*val) <= 0 );

	det = edgeB.cross(center - b);
	val = edgeB.cross(-b);
	sideFlag.set(1, (det*val) <= 0 );

	det = edgeC.cross(center - c);
	val = edgeC.cross(-c);
	sideFlag.set(2, (det*val) <= 0 );
}

bool testShape2D
	( const SWShape2D* shape1, const SWShape2D::Transform& transform1
	, const SWShape2D* shape2, const SWShape2D::Transform& transform2 )
{
	tvec2 simplex[3];

	//! find first simplex
	{
		tvec2 dir, farthest1, farthest2;

		dir = tvec2::axisX;
		shape1->getFarthest( farthest1, dir, transform1 );
		shape2->getFarthest( farthest2, -dir, transform2 );
		simplex[0] = farthest1 - farthest2;

		dir = (-simplex[0]).normal();
		shape1->getFarthest( farthest1, dir, transform1 );
		shape2->getFarthest( farthest2, -dir, transform2 );
		simplex[1] = farthest1 - farthest2;

		tvec2 line = (simplex[1] - simplex[0]);
		if ( line.length() < SW_Epsilon ) return false;

		float kz = line.cross( simplex[0] );
		if ( SWMath.abs(kz) < SW_Epsilon )
		{
			if (  line.dot( -simplex[0] ) < 0 ) return false;
			if ( -line.dot( -simplex[1] ) < 0 ) return false;
			kz = 1;
		}
		dir = line.cross( kz ).normal();
		shape1->getFarthest( farthest1, dir, transform1 );
		shape2->getFarthest( farthest2, -dir, transform2 );
		simplex[2] = farthest1 - farthest2;
	}

	//! try a few times to find overriding using Minkowski Difference and GJK
	tuint trying = 32;
	while ( trying-- )
	{
		float simplesArea = calculateArea(simplex[0], simplex[1], simplex[2]);
		if ( simplesArea == 0 ) return false;
		float originArea = 0;
		originArea += calculateArea( tvec2::zero, simplex[0], simplex[1]);
		originArea += calculateArea( tvec2::zero, simplex[1], simplex[2]);
		originArea += calculateArea( tvec2::zero, simplex[2], simplex[0]);
		
		//! is it close enough
		if ( SWMath.abs(originArea - simplesArea) < SW_Epsilon )
		{
			//! finds normal and depth using EPA.
			//! we only try as much as maxTrying to find them
			const tuint maxTrying = 16;
			tvec2 polytope[maxTrying];
			tvec2 center = tvec2::zero;
			tuint count = 0;

			//! first, fills polytope with simplex that contains the origin.
			center += polytope[count] = simplex[count];++count;
			center += polytope[count] = simplex[count];++count;
			center += polytope[count] = simplex[count];++count;
			center /= count;
			while ( count < maxTrying )
			{
				tuint insertPos = 0;
				tvec2 normal;
				float closest = FLT_MAX;
				for ( tuint i = 0 ; i < count ; ++i )
				{
					tvec2 edge = polytope[(i+1)%count] - polytope[i];
					float kz = edge.cross( center-polytope[i] );
					tvec2 dir = edge.cross(kz).normal();
					float length = dir.dot(polytope[i]);
					if ( length < closest )
					{
						insertPos = i+1;
						normal = dir;
						closest = length;
					}
				}

				//! finds new farthest point.
				tvec2 farthest1, farthest2;
				shape1->getFarthest( farthest1, normal, transform1 );
				shape2->getFarthest( farthest2, -normal, transform2 );
				tvec2 farthest = farthest1 - farthest2;

				//! is it close enough
				float length = normal.dot(farthest);
				if ( SWMath.abs(length - closest) < SW_Epsilon )
				{
					SWLog( "normal: %f, %f", normal.x, normal.y );
					SWLog( "depth: %f", closest );
					break;
				}
				else
				{
					tuint itor = count;
					while ( itor > insertPos )
					{
						polytope[itor--] = polytope[itor];
					}
					polytope[insertPos] = farthest;
					count += 1;
				}
			}

			return true;
		}
		else
		{
			//! finds sides where origin is.
			tflag8 sideFlag;
			calculateSide( sideFlag, simplex[0], simplex[1], simplex[2] );
			tuint count = 0;
			tuint index = 0;

			//! meaning of outside is opposite side of center of polygon in each edges.
			//! if there are two outsides, 
			//! it means that the shapes don't meet each other in case of the convex-shape.
			if ( sideFlag.get(0) ) { index = 0; count += 1;}
			if ( sideFlag.get(1) ) { index = 1; count += 1;}
			if ( sideFlag.get(2) ) { index = 2; count += 1;}
			if ( count >= 2 ) return false;

			//! finds normal to the origin from a edge.
			tvec2 edge = simplex[(index+1)%3]-simplex[index];
			float kz = edge.cross(simplex[index]);
			tvec2 dir = edge.cross( kz ).normal();

			//! finds new farthest point.
			tvec2 farthest1, farthest2;
			shape1->getFarthest( farthest1, dir, transform1 );
			shape2->getFarthest( farthest2, -dir, transform2 );
			simplex[(index+2)%3] = farthest1 - farthest2;
		}
	}
	return false;
}