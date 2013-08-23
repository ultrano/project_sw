//
//  SWTransform.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWTransform.h"
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

	if ( parent ) parent->m_children.push_back( this );

	SWTransform* oldParent = m_parent();
	if ( oldParent ) oldParent->m_children.remove( this );

	m_parent = parent;
	
}

SWTransform* SWTransform::getParent()
{
	return m_parent();
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
		m_worldMat =  parent->m_worldMat * m_worldMat; 
	}
	
}

SWTransform* SWTransform::find( const std::string& query )
{
	ChildTransforms::iterator itor = m_children.begin();
	for ( ; itor != m_children.end() ;++itor )
	{
		SWTransform* child = (*itor)();
		SWGameObject* object = child->owner();
		if ( object->getName() == query ) return child;
	}
	
	return NULL;
}
