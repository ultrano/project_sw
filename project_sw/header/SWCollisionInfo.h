//
//  SWCollisionInfo.h
//  prototype
//
//  Created by 한상운 on 13. 5. 25..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWCollisionInfo_h
#define prototype_SWCollisionInfo_h

#include "SWMemory.h"

class SWGameObject;

/**
 @brief 충돌 발생시의 정보들을 저장.
 */
class SWCollisionInfo : public SWMemory
{
public:

    int           state;   //< 충돌 상태 ( 현재 안쓰임 )
    SWGameObject* suspect; //< 충돌한 상대 객체.
    SWVector2f      point;   //< 충돌로 예상되는 지점.
    SWVector2f      normal;  //< 충돌시의 법선
    float         normalForce; //< 충돌시의 법선항력 ( 현재 안쓰임 )
};

#endif
