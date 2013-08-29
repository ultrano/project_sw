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
#include "SWBehavior.h"
#include "SWLog.h"

SWGameObject::SWGameObject()
	: m_name( "nonamed" )
{
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
	removeComponentAll();
	_make_zero_ref();
}

void SWGameObject::udpate()
{
	SWTransform* trans = getComponent<SWTransform>();
	trans->update();

	ComponentTable copy = m_componentTable;
	ComponentTable::iterator itor = copy.begin();
	for ( ; itor != copy.end() ; ++itor )
	{
		SWBehavior* behavior = swrtti_cast<SWBehavior>( itor->second() );
		if ( behavior ) behavior->onUpdate();
	}
}

SWComponent* SWGameObject::addComponent( SWComponent* comp )
{
	SWHardRef<SWComponent> holder = comp;

	if ( !comp ) return NULL;
	if ( getComponent( comp->queryRtti() ) ) return NULL;

	m_componentTable.insert( std::make_pair( comp->queryRtti(), comp ) );

	comp->m_gameObject = this;
	comp->onAdded();

	return comp;
}

SWComponent* SWGameObject::getComponent( const SWRtti* rtti )
{
	if ( !rtti ) return NULL;
	ComponentTable::iterator itor = m_componentTable.find( rtti );
	return ( itor != m_componentTable.end() )? itor->second() : NULL;
}

void SWGameObject::removeComponent( const SWRtti* rtti )
{
	SWComponent* comp = getComponent( rtti );
	if ( comp ) comp->onRemoved();
	m_componentTable.erase( rtti );
}

void SWGameObject::removeComponentAll()
{
	ComponentTable copy = m_componentTable;
	ComponentTable::iterator itor = copy.begin();
	for ( ; itor != copy.end() ; ++itor ) itor->second()->onRemoved();
	copy.clear();
	m_componentTable.clear();
}
