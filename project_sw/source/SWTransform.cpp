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

	SWHardRef<SWGameObject> object = getGameObject();

	if ( m_parent() ) m_parent()->m_children.remove( object() );
	else SW_GC.getScene()->m_roots.remove( object() );

	if ( parent ) parent->m_children.push_back( object() );
	else SW_GC.getScene()->m_roots.push_back( object() );

	m_parent = parent;
	
}

SWTransform* SWTransform::getParent()
{
	return m_parent();
}

const SWMatrix4x4& SWTransform::getWorldMatrix()
{
	return m_worldMat;
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

void SWTransform::update()
{
	m_worldMat.transform( m_scale, m_rotate, m_position );

	if ( SWTransform* parent = m_parent() )
	{
		m_worldMat = m_worldMat * parent->m_worldMat;
	}

	ChildList::iterator itor = m_children.begin();
	for ( ; itor != m_children.end() ;++itor )
	{
		SWTransform* transform = (*itor)()->getComponent<SWTransform>();
		if ( transform ) transform->update();
	}
}

SWTransform* SWTransform::find( const std::string& name )
{
	ChildList::iterator itor = m_children.begin();
	for ( ; itor != m_children.end() ;++itor )
	{
		SWGameObject* object = (*itor)();
		if ( object->getName() == name ) return object->getComponent<SWTransform>();
	}
	return NULL;
}

void SWTransform::copyChildren( ChildList& transList )
{
	transList = m_children;
}

void SWTransform::onAdded()
{
	SW_GC.getScene()->m_roots.push_back( getGameObject() );
}

void SWTransform::onRemoved()
{
	SWGameObject* object = getGameObject();
	if ( m_parent() )
	{
		m_parent()->m_children.remove( object );
	}
	else
	{
		SWGameScene* scene = SW_GC.getScene();
		scene->m_roots.remove( object );
	}
	ChildList copy = m_children;
	ChildList::iterator itor = copy.begin();
	for ( ; itor != copy.end() ; ++itor )
	{
		(*itor)()->destroy();
	}
	copy.clear();
	m_children.clear();
	

}
