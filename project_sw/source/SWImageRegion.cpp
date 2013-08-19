//
//  SWImageRegion.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 5..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWImageRegion.h"
#include "SWGameContext.h"


SWImageRegion::SWImageRegion( const int& kImg, int kX, int kY, int kWidth, int kHeight )
: img( kImg )
, x( kX ), y( kY )
, width( kWidth ), height( kHeight )
{
    
}

SWImageRegion::SWImageRegion()
: img( NULL )
, x( 0 ), y( 0 )
, width( 0 ), height( 0 )
{
    
}
SWImageRegion::SWImageRegion( const char* filePath
                             , int kX, int kY
                             , int kWidth, int kHeight )
{
    loadImage( filePath, kX, kY, kWidth, kHeight );
}

bool SWImageRegion::loadImage( const char* filePath, int kX, int kY, int kWidth, int kHeight )
{
    //if ( !SW_GC.createImage( &img, filePath ) ) return false;
    x = kX;
    y = kY;
    width = kWidth;
    height = kHeight;
    return true;
}