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

#include <algorithm>

SWGameObject::SWGameObject()
	: m_name( "nonamed" )
{
	addComponent<SWTransform>();
}

SWGameObject::SWGameObject( const std::string& name )
	: m_name( name )
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

	ComponentArray copy = m_components;
	ComponentArray::iterator itor = copy.begin();
	for ( ; itor != copy.end() ; ++itor )
	{
		SWBehavior* behavior = swrtti_cast<SWBehavior>( (*itor)() );
		if ( behavior ) behavior->onUpdate();
	}
}

SWComponent* SWGameObject::addComponent( SWComponent* comp )
{
	SWHardRef<SWComponent> holder = comp;

	if ( !comp ) return NULL;
	if ( SWComponent* existComp = getComponent( comp->queryRtti() ) ) return existComp;

	m_components.push_back( comp );

	comp->gameObject = this;
	comp->onAdded();

	return comp;
}

SWComponent* SWGameObject::getComponent( const SWRtti* rtti )
{
	if ( !rtti ) return NULL;

	for ( int i = 0 ; i < m_components.size() ; ++i )
	{
		SWComponent* comp = (m_components[i])();
		if ( comp->queryRtti() == rtti ) return comp; 
	}
	return NULL;
}

void SWGameObject::removeComponent( const SWRtti* rtti )
{
	if ( rtti == SWTransform::getRtti() ) return;

	SWComponent* comp = getComponent( rtti );
	if ( comp ) comp->onRemoved();
	std::remove( m_components.begin(), m_components.end(), comp );
}

void SWGameObject::removeComponentAll()
{
	ComponentArray::iterator itor = m_components.end();

	do
	{
		--itor;
		SWComponent* comp = (*itor)();
		comp->onRemoved();
		comp->destroy();
	} while ( itor != m_components.begin() );

	m_components.clear();
}
