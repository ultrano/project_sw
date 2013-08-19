//
//  SWCollisionManager.h
//  prototype
//
//  Created by 한상운 on 13. 5. 24..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWCollisionManager_h
#define prototype_SWCollisionManager_h

#include <list>
#include "SWRefCounter.h"

class SWCollider;

/**
 @brief SWCollider 를 관리함.
 SWCollider 의 생성과 충돌 판정을 관리한다.
 */
class SWCollisionManager : public SWRefCountable
{
    typedef std::list< SWHardRef<SWCollider> > ColliderList;
    ColliderList m_colliders;
    
public:
    
    SWCollisionManager();
    ~SWCollisionManager();
    
    SWCollider* allocCollider();
    void        freeCollider( SWCollider* collider );
    
    void        update( float elapsed );
    void        hitTest( float elapsed, SWCollider* suspect1, SWCollider* suspect2 );
};

#endif
