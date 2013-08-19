//
//  SWTransform.h
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWTransform_h
#define prototype_SWTransform_h

#include "SWComponent.h"
#include "SWVector2f.h"

/**
 @brief 객체가 공간 개념을 갖게 한다.
        특별한 경우를 제외하고 가장 기본이 되는 컴포넌트
 */
class SWTransform : public SWComponent
{
    SW_RTTI( SWTransform, SWComponent );
public:
    
    SWVector2f pos;   //< 위치.
    float    rot;   //< 회전.
    SWVector2f scale; //< 비율.
    
    SWTransform( const SWVector2f& kPos = SWVector2f(0,0), float kRot = 0, const SWVector2f& kScale = SWVector2f(1,1) );
    ~SWTransform();
};

#endif
