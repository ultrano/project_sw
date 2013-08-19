//
//  SWNumberDrawer.cpp
//  shakeThatAss
//
//  Created by 한상운 on 13. 6. 17..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWNumberDrawer.h"
#include "SWDefines.h"
#include <math.h>

SWNumberDrawer::SWNumberDrawer()
: m_number( 0 )
, m_align( SW_Align_Left )
{
    
}
void SWNumberDrawer::setNumberRegion( unsigned int num, const SWImageRegion &region )
{
    m_numRegions[num] = region;
}

const SWImageRegion& SWNumberDrawer::getNumberRegion( unsigned int num )
{
    return m_numRegions[num];
}

void SWNumberDrawer::onDraw()
{
    int number = m_number;
    int digit = 1 + ( (number<=0)? 0:log10f( number ) );
    float width = ( m_width * 2 / 3 );
    float stringWidth = digit * width;
    float oldOffsetX = m_offsetX;
    float adjustX = oldOffsetX;
         if ( m_align == SW_Align_Right ) adjustX += stringWidth;
    else if ( m_align == SW_Align_Center ) adjustX += stringWidth/2;
    
    for ( int i = 0 ; i < digit ; ++i )
    {
        int n = number % 10;
        number /= 10;
        m_offsetX = adjustX - ( width * i );
        setRegion( getNumberRegion(n) );
        __super::onDraw();
    }
    m_offsetX = oldOffsetX;
}