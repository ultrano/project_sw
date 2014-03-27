﻿//
//  SWTransform.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWTransform.h"
#include "SWGameContext.h"
#include "SWGameScene.h"
#include "SWGameObject.h"
#include "SWLog.h"
#include "SWParam.h"
#include "SWMath.h"
#include <algorithm>

SWTransform::SWTransform()
: m_position( 0, 0, 0 )
, m_rotate()
, m_euler( 0, 0, 0 )
, m_scale( 1, 1, 1 )
, m_needLocalUpdate( true )
, m_needWorldUpdate( true )
{
}

SWTransform::~SWTransform()
{
    //SW_OutputLog( "transform", "deleted" );
}

void SWTransform::setParent( SWTransform* parent )
{
	if ( m_parent() == parent ) return;

	SWGameObject::Ref object = gameObject();

	if ( m_parent() ) m_parent()->m_children.remove( object() );
	else SW_GC.getScene()->m_roots.remove( object() );

	if ( parent ) parent->m_children.push_back( object() );
	else SW_GC.getScene()->m_roots.push_back( object() );

	m_parent = parent;

	if ( m_setParentDelegates.size() == 0 ) return;
	
	SWArray* arr = new SWArray();
	SWHardRef<SWParam> param = new SWParam( arr );
	arr->add( this );
	arr->add( parent );

	SWObject::List copy = m_setParentDelegates;
	SWObject::List::iterator itor = copy.begin();
	for ( ; itor != copy.end() ;++itor )
	{
		SWDelegator* dg = swrtti_cast<SWDelegator>( (*itor)() );
		if ( dg == NULL ) m_setParentDelegates.remove( *itor );
		dg->call( param() );
	}

	param()->destroy();
}

SWTransform* SWTransform::getParent() const
{
	return m_parent();
}

void SWTransform::addSetParentDelegate( SWDelegator* dg )
{
	m_setParentDelegates.push_back( dg );
}

void SWTransform::removeSetParentDelegate( SWDelegator* dg )
{
	if ( !dg ) return;
	removeSetParentDelegate( dg->getObject(), dg->getHandler() );
}

void SWTransform::removeSetParentDelegate( SWObject* object, const SWHandler& handler )
{
	if ( !object ) return;
	if ( !handler.m_method ) return;
	
	SWObject::List::iterator itor = m_setParentDelegates.begin();
	for ( ; itor != m_setParentDelegates.end() ; ++itor )
	{
		SWDelegator* itorDG = swrtti_cast<SWDelegator>( (*itor)() );
		if ( itorDG->isEqual( object, handler ) )
		{
			m_setParentDelegates.remove( *itor );
			return;
		}
	}
}

const TMatrix4x4& SWTransform::getWorldMatrix()
{
	if ( m_needWorldUpdate )
	{
		m_needWorldUpdate = false;
		updateMatrix();
	}
	return m_worldMat;
}

const TMatrix4x4& SWTransform::getLocalMatrix()
{
	if ( m_needLocalUpdate )
	{
		m_needLocalUpdate = false;
		m_localMat.transform( m_scale, m_rotate, m_position );
	}
	return m_localMat;
}

const TVector3f& SWTransform::getLocalScale() const
{
	return m_scale;
}

const TQuaternion& SWTransform::getLocalRotate() const
{
	return m_rotate;
}

const TVector3f& SWTransform::getLocalPosition() const
{
	return m_position;
}

TVector3f SWTransform::getPosition()
{
	const tmat44& worldMat = getWorldMatrix();
	return tvec3( worldMat.m41, worldMat.m42, worldMat.m43 );
}

void SWTransform::setPosition( const tvec3& pos )
{
	setLocalPosition( worldToLocalPoint( pos ) );
}

tvec3 SWTransform::worldToLocalPoint( const tvec3& point ) const
{
	if ( SWTransform* parent = getParent() )
	{
		return point* parent->m_invWorldMat;
	}
	return point;
}

void SWTransform::setLocalScale( const TVector3f& scale )
{
	m_scale = scale;
	needUpdateMatrix();
}

void SWTransform::setLocalRotate( const TQuaternion& rotate )
{
	m_rotate = rotate;
	m_rotate.toEuler( m_euler );
	needUpdateMatrix();
}

void SWTransform::setLocalRotate( const tvec3& euler )
{
	m_euler = euler;
	m_rotate.rotate( m_euler );
	needUpdateMatrix();
}

void SWTransform::setLocalPosition( const TVector3f& position )
{
	m_position = position;
	needUpdateMatrix();
}

void SWTransform::move( float stepX, float stepY, float stepZ )
{
	move( tvec3( stepX, stepY, stepZ ) );
}

void SWTransform::move( const tvec3& step )
{
	setLocalPosition( m_position + step );
}

void SWTransform::rotate( float radianX, float radianY, float radianZ )
{
	rotate( tvec3( radianX, radianY, radianZ ) );
}

void SWTransform::rotate( const tvec3& euler )
{
	setLocalRotate( m_euler + euler );
}

SWTransform* SWTransform::find( const tstring& name )
{
	SWObject::List::iterator itor = m_children.begin();
	for ( ; itor != m_children.end() ;++itor )
	{
		SWGameObject* object = swrtti_cast<SWGameObject>( (*itor)() );
		if ( object->getName() == name ) return object->getComponent<SWTransform>();
	}
	return NULL;
}

void SWTransform::copyChildren( SWObject::List& transList )
{
	transList = m_children;
}

void SWTransform::onStart()
{
	__super::onStart();
	SW_GC.getScene()->m_roots.push_back( gameObject.getRaw() );
	gameObject()->addUpdateDelegator( GetDelegator( onUpdate ) );
}

void SWTransform::onRemove()
{
	gameObject()->removeUpdateDelegator( GetDelegator( onUpdate ) );
	SWObject::List copy = m_children;
	SWObject::List::iterator itor = copy.begin();
	for ( ; itor != copy.end() ; ++itor )
	{
		SWGameObject* go = swrtti_cast<SWGameObject>( (*itor)() );
		go->destroy();
	}
	copy.clear();
	m_children.clear();
	
	SWGameObject* object = gameObject();
	if ( m_parent() ) m_parent()->m_children.remove( object );
	else SW_GC.getScene()->m_roots.remove( object );
}

void SWTransform::onUpdate()
{
	updateMatrix();

	SWObject::List::iterator itor = m_children.begin();
	for ( ; itor != m_children.end() ;++itor )
	{
		SWGameObject* go = swrtti_cast<SWGameObject>( (*itor)() );
		go->udpate();
	}
}

void SWTransform::onAnimate( const thashstr& key, float value )
{
	static const thashstr posX = "position.x";
	static const thashstr posY = "position.y";
	static const thashstr posZ = "position.z";
	static const thashstr scaleX = "scale.x";
	static const thashstr scaleY = "scale.y";
	static const thashstr scaleZ = "scale.z";
	static const thashstr rotationX = "rotation.x";
	static const thashstr rotationY = "rotation.y";
	static const thashstr rotationZ = "rotation.z";
	/*
	if ( posX == key ) m_position.x = value;
	else if ( posY == key ) m_position.y = value;
	else if ( posZ == key ) m_position.z = value;
	else if ( scaleX == key ) m_scale.x = value;
	else if ( scaleY == key ) m_scale.y = value;
	else if ( scaleZ == key ) m_scale.z = value;
	else if ( rotationX == key )
	{
		m_euler.x = SWMath.angleToRadian( value );
		m_rotate.rotate( m_euler );
	}
	else if ( rotationY == key )
	{
		m_euler.y = SWMath.angleToRadian( value );
		m_rotate.rotate( m_euler );
	}
	else if ( rotationZ == key )
	{
		m_euler.z = SWMath.angleToRadian( value );
		m_rotate.rotate( m_euler );
	}
	*/
}

void SWTransform::needUpdateMatrix()
{
	m_needLocalUpdate = true;
	m_needWorldUpdate = true;
}

void SWTransform::updateMatrix()
{
	if ( SWTransform* parent = m_parent() )
	{
		m_worldMat = getLocalMatrix()* parent->getWorldMatrix();
	}
	else
	{
		m_worldMat = getLocalMatrix();
	}
	m_worldMat.inverse( m_invWorldMat );
}
