//
//  SWGameObject.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWGameObject.h"
#include "SWGameContext.h"
#include "SWGameScene.h"
#include "SWTransform.h"
#include "SWLog.h"

SWGameObject::SWGameObject()
{
    SW_GC.getScene()->m_objects.push_back( this );
	addComponent<SWTransform>();
}

SWGameObject::~SWGameObject()
{
}

void SWGameObject::awake()
{
}

void SWGameObject::destroy()
{
	SWHardRef<SWGameObject> holder = this;
	SW_GC.getScene()->m_objects.remove( this );
}

void SWGameObject::destroyNow()
{
	SWHardRef<SWGameObject> holder = this;
	SW_GC.getScene()->m_objects.remove( this );
	_make_zero_ref();
}

void SWGameObject::addComponent( SWComponent* comp )
{
	if ( !comp ) return;
	SWHardRef<SWComponent> holder = comp;
	m_componentTable.insert( std::make_pair( comp->queryRtti(), comp ) );
	comp->m_gameObject = this;
}

void SWGameObject::removeComponent( const SWRtti* rtti )
{
	if ( !rtti ) return;
	m_componentTable.erase( rtti );
}

SWComponent* SWGameObject::getComponent( const SWRtti* rtti )
{
	if ( !rtti ) return NULL;
	ComponentTable::iterator itor = m_componentTable.find( rtti );
	return ( itor != m_componentTable.end() )? itor->second() : NULL;
}
