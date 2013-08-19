//
//  SWSphereCollider.h
//  prototype
//
//  Created by 한상운 on 13. 5. 28..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWSphereCollider_h
#define prototype_SWSphereCollider_h

#include "SWCollider.h"

class SWSphereCollider : public SWCollider
{
    SW_RTTI( SWSphereCollider, SWCollider );
    
public:
    
    float m_radius; //< 원의 반지름
    
    SWSphereCollider( float radius = 0 ) : m_radius( radius ) { setColliding( true ); };
    void setRadius( float radius ) { m_radius = radius; }
    float getRadius() const { return m_radius; }
    
    
    
};


#endif
