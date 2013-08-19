//
//  SWCollider.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 24..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWCollider.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWDefines.h"
#include "SWCollisionInfo.h"
#include "SWBehavior.h"
#include "SWLog.h"
#include "SWSphereCollider.h"

SWCollider::SWCollider()
: m_mass( 1 )
, m_velocity( 0, 0 )
, m_drag( 0 )
, m_elastic( 1 )
, m_doCollide( false )
{
    
}

SWCollider::~SWCollider()
{
    //SW_OutputLog( "collider", "deleted" );
}

void SWCollider::applyForce( float x, float y )
{
    if ( m_mass <= 0 )
    {
        SW_OutputLog( "collider apply force", "mass can't be zero or less" );
        return;
    }
    m_velocity.x += x / m_mass;
    m_velocity.y += y / m_mass;
}

void SWCollider::applyForce( const SWVector2f &force )
{
    applyForce( force.x, force.y );
}

void SWCollider::applyAccel( float x, float y )
{
    m_velocity.x += x;
    m_velocity.y += y;
}

void SWCollider::applyAccel( const SWVector2f &accel )
{
    m_velocity += accel;
}

void SWCollider::update( float elapsed )
{
    if ( !owner.isValid() ) return;
    
    SWTransform* transform = owner()->getTransform();
    transform->pos += m_velocity * elapsed;
    
    m_velocity -= m_velocity * ( m_drag * elapsed );
    
}

void SWCollider::onCollision( const SWCollisionInfo& info )
{
    if ( !owner.isValid() ) return;
    if ( SWBehavior* behavior = owner()->getBehavior() ) behavior->onCollision( info );
    
}

void SWCollider::collideWith( const SWVector2f &normal, const SWVector2f &velocity, float mass )
{
    
    float v1p   = normal.dotWith( m_velocity );
    float v2p   = normal.dotWith( velocity );
    float mass1 = m_mass;
    float mass2 = mass;
    float e     = m_elastic;
    float v1pl  = ( ( mass1 - e * mass2 ) * v1p + (1 + e) * mass2 * v2p ) / (mass1 + mass2);
    
    m_velocity  = m_velocity + ( normal * (v1pl - v1p) );
    
}

void SWCollider::collisionTest( SWCollider *suspect1, SWCollider *suspect2 )
{
    SWTransform* trans1 = suspect1->owner()->getTransform();
    SWTransform* trans2 = suspect2->owner()->getTransform();
    float distance = trans1->pos.distWith( trans2->pos );
    
    SWSphereCollider* sphere1 = swrtti_cast<SWSphereCollider>( suspect1 );
    SWSphereCollider* sphere2 = swrtti_cast<SWSphereCollider>( suspect2 );
    
    SWCollisionInfo info;
    if ( distance <= ( sphere1->getRadius() + sphere2->getRadius() ) )
    {
        SWVector2f point  = (trans1->pos + trans2->pos) / 2.0f;
        SWVector2f normal = (trans1->pos - trans2->pos).normalize();
        float mass1 = suspect1->getMass();
        float mass2 = suspect2->getMass();
        SWVector2f velocity1 = suspect1->getVelocity();
        SWVector2f velocity2 = suspect2->getVelocity();
        
        suspect1->collideWith( normal, velocity2, mass2 );
        suspect2->collideWith( normal, velocity1, mass1 );
        
        float momentum1 = ( suspect1->getVelocity() * suspect1->getMass() ).length();
        float momentum2 = ( suspect2->getVelocity() * suspect2->getMass() ).length();
        float depth = ( sphere1->getRadius() + sphere2->getRadius() ) - distance;
        trans1->pos = trans1->pos + (normal * depth * ( momentum1 / ( momentum1 + momentum2 + 0.1f) ) );
        trans2->pos = trans2->pos - (normal * depth * ( momentum2 / ( momentum1 + momentum2 + 0.1f) ) );
        
        info.point  = point;
        
        info.normal = -normal;
        info.suspect = suspect2->owner();
        suspect1->onCollision( info );
        
        info.normal = normal;
        info.suspect = suspect1->owner();
        suspect2->onCollision( info );
    }
}