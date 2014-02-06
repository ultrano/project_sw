//
//  TVector2f.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "TVector2f.h"
#include "SWLog.h"
#include <math.h>

TVector2f::TVector2f()
{
    
}

TVector2f::TVector2f( float kx, float ky )
: x( kx )
, y( ky )
{
    
}

float TVector2f::distWith( float kx, float ky ) const
{
    kx = x - kx;
    ky = y - ky;
    return sqrtf( ( kx * kx ) + ( ky * ky ) );
}

float TVector2f::distWith( const TVector2f &pt ) const
{
    return distWith( pt.x, pt.y );
}

float TVector2f::length() const
{
    return sqrtf( x*x + y*y );
    // or
    // return distWith( 0, 0 );
}

float TVector2f::dotWith( float kx, float ky ) const
{
    return ( x*kx + y*ky );
}

float TVector2f::dotWith( const TVector2f &pt ) const
{
    return dotWith( pt.x, pt.y );
}

TVector2f   TVector2f::normalize() const
{
    TVector2f pt( 0, 0 );
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

TVector2f    TVector2f::operator +( const TVector2f& pt ) const
{
    return TVector2f( x + pt.x, y + pt.y );
}

TVector2f    TVector2f::operator -( const TVector2f& pt ) const
{
    return TVector2f( x - pt.x, y - pt.y );
}

TVector2f    TVector2f::operator -() const
{
    return TVector2f( -x, -y );
}

TVector2f    TVector2f::operator *( float scalar ) const
{
    return TVector2f( x * scalar, y * scalar );
}

TVector2f    TVector2f::operator /( float scalar ) const
{
    return TVector2f( x / scalar, y / scalar );
}

TVector2f&   TVector2f::operator +=( const TVector2f& pt )
{
    x += pt.x;
    y += pt.y;
    return *this;
}

TVector2f&   TVector2f::operator -=( const TVector2f& pt )
{
    x -= pt.x;
    y -= pt.y;
    return *this;
}

bool        TVector2f::operator ==( const TVector2f& pt ) const
{
    return ( ( x == pt.x ) && ( y == pt.y ) );
}

bool        TVector2f::operator !=( const TVector2f& pt ) const
{
    return ( ( x != pt.x ) && ( y != pt.y ) );
}