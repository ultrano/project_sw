//
//  SWBound.h
//  prototype
//
//  Created by 한상운 on 13. 5. 24..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWCollider_h
#define prototype_SWCollider_h

#include "SWComponent.h"
#include "SWVector2f.h"

class SWGameObject;
class SWCollisionInfo;

/** 
 @brief 충돌 및 물리 처리를 담당하는 컴포넌트
 SWCollisionManager 에 의해 생성 및 관리된다.
 */
class SWCollider : public SWComponent
{
    SW_RTTI( SWCollider, SWComponent );
private:
    
    //! 물리 관련 
    float    m_mass;     //< 질량
    SWVector2f m_velocity; //< 속도
    float    m_drag;     //< 저항
    float    m_elastic;  //< 탄성
    
    bool     m_doCollide; //< 충돌 검사 여부
    
public:
    
    //! 함수들
    SWCollider();
    ~SWCollider();
    
    void setMass( float mass ) { m_mass = mass; }
    float getMass() const { return m_mass; }
    
    void setDrag( float drag ) { m_drag = drag; }
    float getDrag() const { return m_drag; }
    
    void setElastic( float elastic ) { m_elastic = elastic; }
    float getElastic() const { return m_elastic; }
    
    void setVelocity( const SWVector2f& velocity ) { setVelocity( velocity.x, velocity.y ); }
    void setVelocity( float x, float y ) { m_velocity.x = x; m_velocity.y = y; }
    const SWVector2f& getVelocity() { return m_velocity; };
    
    void applyForce( float x, float y );
    void applyForce( const SWVector2f& force );
    
    void applyAccel( float x, float y );
    void applyAccel( const SWVector2f& accel );
    
    bool doCollide() { return m_doCollide; };
    void setColliding( bool check ) { m_doCollide = check; };
    
    void collideWith( const SWVector2f& normal, const SWVector2f& velocity, float mass );
    
    void update( float elapsed );
    
    void onCollision( const SWCollisionInfo& info );
    
    static void collisionTest( SWCollider* suspect1, SWCollider* suspect2 );
    
};

#endif
