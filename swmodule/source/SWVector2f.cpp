//
//  SWVector2f.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWVector2f.h"
#include "SWLog.h"
#include <math.h>

SWVector2f::SWVector2f()
{
    
}

SWVector2f::SWVector2f( float kx, float ky )
: x( kx )
, y( ky )
{
    
}

float SWVector2f::distWith( float kx, float ky ) const
{
    kx = x - kx;
    ky = y - ky;
    return sqrtf( ( kx * kx ) + ( ky * ky ) );
}

float SWVector2f::distWith( const SWVector2f &pt ) const
{
    return distWith( pt.x, pt.y );
}

float SWVector2f::length() const
{
    return sqrtf( x*x + y*y );
    // or
    // return distWith( 0, 0 );
}

float SWVector2f::dotWith( float kx, float ky ) const
{
    return ( x*kx + y*ky );
}

float SWVector2f::dotWith( const SWVector2f &pt ) const
{
    return dotWith( pt.x, pt.y );
}

SWVector2f   SWVector2f::normalize() const
{
    SWVector2f pt( 0, 0 );
    float len = length();
    if ( len != 0.0f )
    {
        pt.x = x / len;
        pt.y = y / len;
    }
    else
    {
        //! 길이가 0인 벡터에 대한 단위화는 보장하지 않는다. (사전에 체크)
       
        //SWLog( "zero length vector" );
    }
    return pt;
}

SWVector2f    SWVector2f::operator +( const SWVector2f& pt ) const
{
    return SWVector2f( x + pt.x, y + pt.y );
}

SWVector2f    SWVector2f::operator -( const SWVector2f& pt ) const
{
    return SWVector2f( x - pt.x, y - pt.y );
}

SWVector2f    SWVector2f::operator -() const
{
    return SWVector2f( -x, -y );
}

SWVector2f    SWVector2f::operator *( float scalar ) const
{
    return SWVector2f( x * scalar, y * scalar );
}

SWVector2f    SWVector2f::operator /( float scalar ) const
{
    return SWVector2f( x / scalar, y / scalar );
}

SWVector2f&   SWVector2f::operator +=( const SWVector2f& pt )
{
    x += pt.x;
    y += pt.y;
    return *this;
}

SWVector2f&   SWVector2f::operator -=( const SWVector2f& pt )
{
    x -= pt.x;
    y -= pt.y;
    return *this;
}

bool        SWVector2f::operator ==( const SWVector2f& pt ) const
{
    return ( ( x == pt.x ) && ( y == pt.y ) );
}

bool        SWVector2f::operator !=( const SWVector2f& pt ) const
{
    return ( ( x != pt.x ) && ( y != pt.y ) );
}