//
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
#include <algorithm>

SWTransform::SWTransform()
: m_position( 0, 0, 0 )
, m_rotate()
, m_scale( 1, 1, 1 )
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

SWTransform* SWTransform::getParent()
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
	return m_worldMat;
}

TMatrix4x4 SWTransform::getLocalMatrix()
{
	return TMatrix4x4().transform( m_scale, m_rotate, m_position );
}

const TVector3f& SWTransform::getLocalScale()
{
	return m_scale;
}

const TQuaternion& SWTransform::getLocalRotate()
{
	return m_rotate;
}

const TVector3f& SWTransform::getLocalPosition()
{
	return m_position;
}

void SWTransform::setLocalScale( const TVector3f& scale )
{
	m_scale = scale;
}

void SWTransform::setLocalRotate( const TQuaternion& rotate )
{
	m_rotate = rotate;
}

void SWTransform::setLocalPosition( const TVector3f& position )
{
	m_position = position;
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

void SWTransform::onUpdate( SWGameObject* )
{
	m_worldMat.transform( m_scale, m_rotate, m_position );

	if ( SWTransform* parent = m_parent() )
	{
		m_worldMat = m_worldMat * parent->getWorldMatrix();
	}

	SWObject::List::iterator itor = m_children.begin();
	for ( ; itor != m_children.end() ;++itor )
	{
		SWGameObject* go = swrtti_cast<SWGameObject>( (*itor)() );
		go->udpate();
	}
}
