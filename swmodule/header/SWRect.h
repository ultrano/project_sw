//
//  SWRect.h
//  prototype
//
//  Created by 한상운 on 13. 5. 24..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWRect_h
#define prototype_SWRect_h

#include "SWMemory.h"
#include "TVector2f.h"

class SWRect : public SWMemory
{
public:
    float left, top, right, bottom;
    
    SWRect();
    SWRect( float l, float t, float r, float b );
    
    /**
     @return SW_RectIn, SW_RectOut, SW_RectRound 중에서 리턴
     */
    int hitTest( float x, float y );
    
    //! @return Rect 크리
    float width();
    float height();
    TVector2f size();
    
    //! 가로, 세로 중앙값
    float centerX();
    float centerY();
    TVector2f center();
};

#endif
