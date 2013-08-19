//
//  SWCollisionManager.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 24..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWCollisionManager.h"
#include "SWCollider.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWDefines.h"
#include "SWCollisionInfo.h"

SWCollisionManager::SWCollisionManager()
{
}

SWCollisionManager::~SWCollisionManager()
{
    
}

SWCollider* SWCollisionManager::allocCollider()
{
    SWCollider* collider = new SWCollider;
    
    m_colliders.push_back( collider );
    
    return collider;
    
}

void SWCollisionManager::freeCollider( SWCollider* collider )
{
    m_colliders.remove( collider );
}

void SWCollisionManager::update( float elapsed )
{
    if ( m_colliders.size() == 0 ) return;
    
    for ( ColliderList::iterator itor1 = m_colliders.begin()
         ; itor1 != m_colliders.end()
         ; ++itor1 )
    {
        SWCollider* target = (*itor1)();
        
        target->update( elapsed );

        ColliderList::iterator itor2 = itor1;
        ++itor2;
        for ( ; itor2 != m_colliders.end() ; ++itor2 )
        {
            hitTest( elapsed, target, (*itor2)() );
        }
        
    }
}

void SWCollisionManager::hitTest( float elapsed, SWCollider *suspect1, SWCollider *suspect2 )
{
    
}