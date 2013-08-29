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

	SWTransform* trans = addComponent<SWTransform>();
	SWGameObject* root = SW_GC.getScene()->m_root();

	trans->setParent( root->getComponent<SWTransform>();
}

SWGameObject::~SWGameObject()
{
	SWTransform* trans = addComponent<SWTransform>();
	trans->setParent( NULL );
}

void SWGameObject::awake()
{
}

void SWGameObject::destroy()
{
	SWHardRef<SWGameObject> holder = this;
}

SWGameObject* SWGameObject::find( const std::string& query )
{
	return NULL;
}

SWComponent* SWGameObject::addComponent( SWComponent* comp )
{
	if ( !comp ) return NULL;
	if ( getComponent( comp->queryRtti() ) ) return NULL;
	SWHardRef<SWComponent> holder = comp;
	m_componentTable.insert( std::make_pair( comp->queryRtti(), comp ) );
	comp->m_gameObject = this;
	return comp;
}

SWComponent* SWGameObject::getComponent( const SWRtti* rtti )
{
	if ( !rtti ) return NULL;
	ComponentTable::iterator itor = m_componentTable.find( rtti );
	return ( itor != m_componentTable.end() )? itor->second() : NULL;
}
