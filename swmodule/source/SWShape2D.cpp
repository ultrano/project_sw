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

bool SWCircleShape2D::getFarthest( tvec2& farthest, const tvec2& direction, const tmat33& mat ) const
{
	tmat33 inv;
	mat.inverse( inv );
	tvec2 localDIr = (tvec3(direction,0) * inv).xy().normal();
	tvec2 local = (localDIr * m_radius) + m_center;
	farthest = local * mat;

	return true;
}

bool SWCircleShape2D::getCrossest( tvec2& begin, tvec2& end, const tvec2& direction, const tmat33& mat ) const
{
	tvec2 middle;
	getFarthest( middle, direction, mat );
	tvec2 gap = direction.cross( 1 ) * (SW_Epsilon/3.0f);
	begin = middle - gap;
	end   = middle + gap;
	return true;
}

void SWCircleShape2D::computeAABB( taabb2d& aabb, const tmat33& mat ) const
{
	aabb.lower = tvec2( FLT_MAX, FLT_MAX );
	aabb.upper = -aabb.lower;

	tmat33 inv;
	mat.inverse( inv );

	const tuint count = 4;
	tvec2 dirs[count] = { tvec2::axisX, tvec2::axisY, -tvec2::axisX, -tvec2::axisY };
	for ( tuint i = 0 ; i < count ; ++i )
	{
		tvec2 localDIr = (tvec3(dirs[i],0) * inv).xy().normal();
		tvec2 point = (localDIr * m_radius) + m_center;

		point = point * mat;
		aabb.min( point );
		aabb.max( point );
	}
}

void SWCircleShape2D::computeMass( SWMassData& data, float density ) const
{
	data.mass = density * (SWMath.pi * m_radius * m_radius);
	data.center = m_center;
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

	tuint count = vertices.size();
	m_normals.resize( count );
	m_vertices.resize( count );

	for ( tuint i = 0 ; i < vertices.size() ; ++i )
	{
		tuint i1 = i;
		tuint i2 = ((i+1)%count);
		
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

bool SWPolygonShape2D::getFarthest( tvec2& farthest, const tvec2& direction, const tmat33& mat ) const
{
	if ( m_vertices.size() == 0 ) return false;

	tmat33 inv;
	mat.inverse( inv );
	tvec3 test = (tvec3(direction,0) * inv);
	tvec2 localDIr = test.xy().normal();

	tuint index = 0;
	float maxDist = localDIr.dot( m_vertices[index] );

	tuint count = m_vertices.size();
	while ( count-- )
	{
		const tvec2& v = m_vertices[count];
		float dist = localDIr.dot( v );
		if ( dist > maxDist )
		{
			index = count;
			maxDist = dist;
		}
	}

	farthest = m_vertices[index] * mat;

	return true;
}

bool SWPolygonShape2D::getCrossest( tvec2& begin, tvec2& end, const tvec2& direction, const tmat33& mat ) const
{
	if ( m_normals.size() < 3 ) return false;

	tmat33 inv;
	mat.inverse( inv );
	tvec2 localDIr = (tvec3(direction,0) * inv).xy().normal();

	tuint index = 0;
	float maxGap = localDIr.dot( m_normals[index] );
	tuint count = m_normals.size();
	for ( tuint i = 0 ; i < count ; ++i )
	{
		float gap = localDIr.dot( m_normals[i] );
		if ( gap > maxGap )
		{
			maxGap = gap;
			index = i;
		}
	}

	begin = m_vertices[index] * mat;
	end   = m_vertices[(index+1)%count] * mat;

	return true;
}

void SWPolygonShape2D::computeAABB( taabb2d& aabb, const tmat33& mat ) const
{
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

void SWPolygonShape2D::computeMass( SWMassData& data, float density ) const
{
	float mass = 0;
	float area = 0;
	tvec2 center(0,0);
	const tuint count = m_vertices.size();
	for ( tuint i = 0 ; i < count ; ++i )
	{
		tuint i1 = i;
		tuint i2 = ((i+1)%count);

		const tvec2& v1 = m_vertices.at( i1 );
		const tvec2& v2 = m_vertices.at( i2 );
		float V = 0.5f * v1.cross(v2);
		tvec2 centroid = (v1+v2)/3.0f;
		center += V * centroid;
		area += V;
	}

	data.mass   = density*area;
	data.center = center/area;
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

bool clipToEdge( tvec2& v1, tvec2& v2, const tvec2& edge1, const tvec2& edge2 )
{
	tvec2 dirE = (edge2 - edge1);
	float eLen = dirE.length();
	tvec2 dirV = (v2-v1);
	float vLen = dirV.length();

	dirE = dirE.normal();
	dirV = dirV.normal();
	
	float d1, d2, len, rate;
	tvec2 term1, term2;
	
	d1 = dirE.dot( v1-edge1 );
	d2 = dirE.dot( v2-edge1 );

	if ( d1 < 0 )
	{
		len = d1 - d2;
		rate = d1/len;
		v1 += rate*vLen*dirV;
	}
	if ( d2 < 0 )
	{
		len = d2 - d1;
		rate = d2/len;
		v2 -= rate*vLen*dirV;
	}
	if ( d1 > eLen )
	{
		len = d1 - d2;
		rate = (d1-eLen)/len;
		v1 += rate*vLen*dirV;
	}
	if ( d2 > eLen )
	{
		len = d2 - d1;
		rate = (d2-eLen)/len;
		v2 -= rate*vLen*dirV;
	}
	return true;
}

bool testShape2D
	( SWManifold& manifold
	, const SWShape2D* shape1, const tmat33& mat1
	, const SWShape2D* shape2, const tmat33& mat2 )
{
	tvec2 simplex[3];

	//! find first simplex
	{
		tvec2 dir, farthest1, farthest2;

		dir = tvec2::axisX;
		shape1->getFarthest( farthest1, dir, mat1 );
		shape2->getFarthest( farthest2, -dir, mat2 );
		simplex[0] = farthest1 - farthest2;

		dir = (-simplex[0]).normal();
		shape1->getFarthest( farthest1, dir, mat1 );
		shape2->getFarthest( farthest2, -dir, mat2 );
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
		shape1->getFarthest( farthest1, dir, mat1 );
		shape2->getFarthest( farthest2, -dir, mat2 );
		simplex[2] = farthest1 - farthest2;
	}

	//! try a few times to find overriding using Minkowski Difference and GJK
	tuint trying = 32;
	while ( trying-- )
	{
		float simplesArea = calculateArea(simplex[0], simplex[1], simplex[2]);
		if ( simplesArea < SW_Epsilon ) return false;
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
				tvec2 direction;
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
						direction = dir;
						closest = length;
					}
				}

				//! finds new farthest point.
				tvec2 farthest1, farthest2;
				shape1->getFarthest( farthest1, direction, mat1 );
				shape2->getFarthest( farthest2, -direction, mat2 );
				tvec2 farthest = farthest1 - farthest2;

				//! save the result
				manifold.normal = direction;
				manifold.depth = closest;

				//! is it close enough
				float length = direction.dot(farthest);
				if ( SWMath.abs(length - closest) < SW_Epsilon )
				{
					manifold.normal = manifold.normal.normal();
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

			//! finds incident points using edge clipping.
			{
				manifold.count = 0;
				float length = 0;
				struct {tvec2 v1,v2;} ref, inc;
				shape1->getCrossest( ref.v1, ref.v2, manifold.normal, mat1);
				if ( (ref.v1-ref.v2).length() < SW_Epsilon ) manifold.points[manifold.count++] = (ref.v1+ref.v2)*0.5f;
				shape2->getCrossest( inc.v1, inc.v2, -manifold.normal, mat2);
				if ( (inc.v1-inc.v2).length() < SW_Epsilon ) manifold.points[manifold.count++] = (inc.v1+inc.v2)*0.5f;

				if ( manifold.count == 0 )
				{
					clipToEdge( inc.v1, inc.v2, ref.v1, ref.v2 );
					tvec2 edgeNormal = (ref.v2 - ref.v1).normal();
					edgeNormal = -edgeNormal.cross( edgeNormal.cross(manifold.normal) );

					float d1, d2;
					d1 = edgeNormal.dot( inc.v1 - ref.v1 );
					d2 = edgeNormal.dot( inc.v2 - ref.v1 );
					if ( d1 < 0 ) manifold.points[manifold.count++] = inc.v1;
					if ( d2 < 0 ) manifold.points[manifold.count++] = inc.v2;
					if ( manifold.count == 2 )
					{
						if ( (inc.v2-inc.v1).length() < SW_Epsilon )
						{
							manifold.count = 1;
							manifold.points[0] = (inc.v2+inc.v1)*0.5f;
						}
					}
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
			shape1->getFarthest( farthest1, dir, mat1 );
			shape2->getFarthest( farthest2, -dir, mat2 );
			simplex[(index+2)%3] = farthest1 - farthest2;
		}
	}
	return false;
}