//
//  SWRect.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 24..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWRect.h"
#include "SWDefines.h"

SWRect::SWRect()
: left( 0 )
, top( 0 )
, right( 0 )
, bottom( 0 )
{
    
}

SWRect::SWRect( float l, float t, float r, float b )
: left( l )
, top( t )
, right( r )
, bottom( b )
{
    
}

int SWRect::hitTest( float x, float y )
{
    if ( ( x == left || x == right ) && ( y == top || y == bottom ) ) return SW_RectRound;
    else if( ( x > left && x < right ) && ( y > top && y < bottom ) ) return SW_RectIn;
    return SW_RectOut;
}

float SWRect::width()
{
    return ( right - left );
}

float SWRect::height()
{
    return ( bottom - top );
}

TVector2f SWRect::size()
{
    return TVector2f( width(), height() );
}

float SWRect::centerX()
{
    return ( right + left ) / 2.0f;
}

float SWRect::centerY()
{
    return ( bottom + top ) / 2.0f;
}

TVector2f SWRect::center()
{
    return TVector2f( centerX(), centerY() );
}