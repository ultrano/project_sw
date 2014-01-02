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

	SWHardRef<SWGameObject> object = gameObject();

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

	SWObjectList copy = m_setParentDelegates;
	SWObjectList::iterator itor = copy.begin();
	for ( ; itor != copy.end() ;++itor )
	{
		SWDelegate* dg = swrtti_cast<SWDelegate>( (*itor)() );
		if ( dg == NULL ) m_setParentDelegates.remove( *itor );
		dg->call( param() );
	}

	param()->destroy();
}

SWTransform* SWTransform::getParent()
{
	return m_parent();
}

void SWTransform::addSetParentDelegate( SWDelegate* dg )
{
	m_setParentDelegates.push_back( dg );
}

void SWTransform::removeSetParentDelegate( SWDelegate* dg )
{
	if ( !dg ) return;
	removeSetParentDelegate( dg->getObject(), dg->getHandler() );
}

void SWTransform::removeSetParentDelegate( SWObject* object, const SWHandler& handler )
{
	if ( !object ) return;
	if ( !handler.m_method ) return;
	
	SWObjectList::iterator itor = m_setParentDelegates.begin();
	for ( ; itor != m_setParentDelegates.end() ; ++itor )
	{
		SWDelegate* itorDG = swrtti_cast<SWDelegate>( (*itor)() );
		if ( itorDG->isEqual( object, handler ) )
		{
			m_setParentDelegates.remove( *itor );
			return;
		}
	}
}

const SWMatrix4x4& SWTransform::getWorldMatrix()
{
	return m_parent.isValid()? m_parent()->getFinalMatrix() : SWMatrix4x4::unit;
}

const SWMatrix4x4& SWTransform::getFinalMatrix()
{
	return m_finalMat;
}

const SWVector3f& SWTransform::getLocalScale()
{
	return m_scale;
}

const SWQuaternion& SWTransform::getLocalRotate()
{
	return m_rotate;
}

const SWVector3f& SWTransform::getLocalPosition()
{
	return m_position;
}

void SWTransform::setLocalScale( const SWVector3f& scale )
{
	m_scale = scale;
}

void SWTransform::setLocalRotate( const SWQuaternion& rotate )
{
	m_rotate = rotate;
}

void SWTransform::setLocalPosition( const SWVector3f& position )
{
	m_position = position;
}

SWTransform* SWTransform::find( const tstring& name )
{
	SWObjectList::iterator itor = m_children.begin();
	for ( ; itor != m_children.end() ;++itor )
	{
		SWGameObject* object = swrtti_cast<SWGameObject>( (*itor)() );
		if ( object->getName() == name ) return object->getComponent<SWTransform>();
	}
	return NULL;
}

void SWTransform::copyChildren( SWObjectList& transList )
{
	transList = m_children;
}

void SWTransform::onStart()
{
	__super::onStart();
	SW_GC.getScene()->m_roots.push_back( gameObject.getRaw() );
	gameObject()->addUpdateDelegate( GetDelegate( onUpdate ) );
}

void SWTransform::onRemove()
{
	gameObject()->removeUpdateDelegate( GetDelegate( onUpdate ) );
	SWObjectList copy = m_children;
	SWObjectList::iterator itor = copy.begin();
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
	m_finalMat.transform( m_scale, m_rotate, m_position );

	if ( SWTransform* parent = m_parent() )
	{
		m_finalMat = m_finalMat * parent->m_finalMat;
	}

	SWObjectList::iterator itor = m_children.begin();
	for ( ; itor != m_children.end() ;++itor )
	{
		SWGameObject* go = swrtti_cast<SWGameObject>( (*itor)() );
		go->udpate();
	}
}
