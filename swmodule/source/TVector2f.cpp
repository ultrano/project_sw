﻿//
//  TVector2f.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "TVector2f.h"
#include "SWLog.h"
#include "SWMath.h"
#include <math.h>

TVector2f::TVector2f()
{
    
}

TVector2f::TVector2f( float kx, float ky )
: x( kx )
, y( ky )
{
    
}

float TVector2f::length() const
{
    return sqrtf( x*x + y*y );
    // or
    // return distWith( 0, 0 );
}

float TVector2f::dist( float kx, float ky ) const
{
    kx = x - kx;
    ky = y - ky;
    return sqrtf( ( kx * kx ) + ( ky * ky ) );
}

float TVector2f::dist( const TVector2f &pt ) const
{
    return dist( pt.x, pt.y );
}

float TVector2f::dot( float kx, float ky ) const
{
    return ( x*kx + y*ky );
}

float TVector2f::dot( const TVector2f &pt ) const
{
    return dot( pt.x, pt.y );
}

float TVector2f::cross( float kx, float ky ) const
{
	return ( x*ky - y*kx );
}

float TVector2f::cross( const TVector2f &pt ) const
{
    return cross( pt.x, pt.y );
}

TVector2f   TVector2f::normal() const
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

void TVector2f::rotateZ( float radian )
{
	float cosR = SWMath.cos( radian );
	float sinR = SWMath.sin( radian );
	float ax = (x * cosR) - (y * sinR);
	float ay = (x * sinR) + (y * cosR);
	x = ax;
	y = ay;
}