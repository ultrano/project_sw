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
#include "SWProfiler.h"

#include <algorithm>

class SWProperty : public SWObject
{
public:
	SWHardRef<SWObject> m_value;
	SWObjectList m_setDelegates;
};

SWGameObject::SWGameObject()
	: m_name( "nonamed" )
{
	SWTransform* comp = new SWTransform;
	m_components.push_back( comp );
	comp->gameObject = this;
	comp->onStart();
}

SWGameObject::SWGameObject( const tstring& name )
	: m_name( name )
{
	addComponent<SWTransform>();
}

SWGameObject::~SWGameObject()
{
	SWLog( "delete game object [%s:%d]", m_name.c_str(), getID() );
}

void SWGameObject::awake()
{
}

void SWGameObject::destroy()
{
	SWHardRef<SWGameObject> holder = this;
	removeComponentAll();

	m_propTable.clear();

	__super::destroy();
}

void SWGameObject::udpate()
{
	if ( m_addedComponents.size() )
	{
		SWObjectList copy = m_addedComponents;
		m_addedComponents.clear();
		SWObjectList::iterator itor = copy.begin();
		for ( ; itor != copy.end() ; ++itor )
		{
			SWComponent* comp = swrtti_cast<SWComponent>( (*itor)() );
			comp->onStart();
		}
	}

	if ( m_updateDelegates.size() )
	{
		SWWeakRef<SWGameObject> vital = this;
		SWObjectList copy = m_updateDelegates;
		SWObjectList::iterator itor = copy.begin();
		for ( ; itor != copy.end() ; ++itor )
		{
			SWDelegate* itorDG = swrtti_cast<SWDelegate>( (*itor)() );
			if ( !itorDG || !itorDG->isValid() ) removeUpdateDelegate( itorDG );
			else itorDG->call( this );
			if ( !vital.isValid() ) return;
		}
	}
}

SWComponent* SWGameObject::addComponent( SWComponent* comp )
{
	SWHardRef<SWComponent> holder = comp;

	if ( !comp ) return NULL;
	if ( SWComponent* existComp = getComponent( comp->queryRtti() ) ) return existComp;

	m_components.push_back( comp );

	comp->gameObject = this;
	m_addedComponents.push_back( comp );
	comp->onAwake();

	return comp;
}

SWComponent* SWGameObject::getComponent( const SWRtti* rtti ) const
{
	if ( !rtti ) return NULL;

	for ( int i = 0 ; i < m_components.size() ; ++i )
	{
		SWComponent* comp = swrtti_cast<SWComponent>( (m_components[i])() );
		if ( comp->queryRtti() == rtti ) return comp; 
	}
	return NULL;
}

void SWGameObject::removeComponent( const SWRtti* rtti )
{
	if ( rtti == SWTransform::getRtti() ) return;

	SWComponent* comp = getComponent( rtti );
	if ( comp ) comp->onRemove();
	std::remove( m_components.begin(), m_components.end(), comp );
}

void SWGameObject::removeComponentAll()
{
	SWObjectArray copy = m_components;
	SWObjectArray::iterator itor = copy.end();
	m_components.clear();

	do
	{
		--itor;
		SWComponent* comp = swrtti_cast<SWComponent>( (*itor)() );
		comp->onRemove();
		comp->destroy();
	} while ( itor != copy.begin() );

}

void SWGameObject::addUpdateDelegate( SWDelegate* dg )
{
	if ( !dg ) return;
	SWObjectList::iterator itor = m_updateDelegates.begin();
	for ( ; itor != m_updateDelegates.end() ; ++itor )
	{
		SWDelegate* itorDG = swrtti_cast<SWDelegate>( (*itor)() );
		if ( itorDG->isEqual( dg ) ) return;
	}
	m_updateDelegates.push_back( dg );
}

void SWGameObject::removeUpdateDelegate( SWDelegate* dg )
{
	if ( !dg ) return;
	m_updateDelegates.remove( dg );
}

void SWGameObject::defineProp( const tstring& name )
{
	if ( isDefinedProp(name) ) return;
	m_propTable.insert( std::make_pair( name, new SWProperty ) );
}

bool SWGameObject::isDefinedProp( const tstring& name )
{
	return ( m_propTable.find( name ) != m_propTable.end() );
}

void SWGameObject::setProp( const tstring& name, SWObject* value )
{
	SWProperty* prop = NULL;
	{
		ObjectMap::iterator itor = m_propTable.find( name );
		if ( itor == m_propTable.end() ) return;

		prop = swrtti_cast<SWProperty>( (itor->second)() );
	}
	if ( prop == NULL ) return;
	
	{
		prop->m_value = value;
		SWObjectList::iterator itor = prop->m_setDelegates.begin();
		for ( ; itor != prop->m_setDelegates.end() ; ++itor )
		{
			SWDelegate* del = swrtti_cast<SWDelegate>( (*itor)() );
			del->call( value );
		}
	}
}

SWObject* SWGameObject::getProp( const tstring& name )
{
	ObjectMap::iterator itor = m_propTable.find( name );
	if ( itor == m_propTable.end() ) return NULL;
	SWProperty* prop = swrtti_cast<SWProperty>( (itor->second)() );
	return prop->m_value();
}

void SWGameObject::addPropSetDelegate( const tstring& name, SWDelegate* del )
{
	if ( del == NULL ) return;
	SWProperty* prop = NULL;
	{
		ObjectMap::iterator itor = m_propTable.find( name );
		if ( itor == m_propTable.end() ) return;

		prop = swrtti_cast<SWProperty>( (itor->second)() );
	}
	if ( prop == NULL ) return;

	prop->m_setDelegates.push_back( del );
}

void SWGameObject::removePropSetDelegate( const tstring& name, SWDelegate* del )
{
	if ( del == NULL ) return;
	SWProperty* prop = NULL;
	{
		ObjectMap::iterator itor = m_propTable.find( name );
		if ( itor == m_propTable.end() ) return;

		prop = swrtti_cast<SWProperty>( (itor->second)() );
	}
	if ( prop == NULL ) return;

	prop->m_setDelegates.remove( del );
}

void SWGameObject::removeAllPropSetDelegate( const tstring& name )
{
	SWProperty* prop = NULL;
	{
		ObjectMap::iterator itor = m_propTable.find( name );
		if ( itor == m_propTable.end() ) return;

		prop = swrtti_cast<SWProperty>( (itor->second)() );
	}
	if ( prop == NULL ) return;

	prop->m_setDelegates.clear();
}

void SWGameObject::cleanPropSetDelegate( const tstring& name )
{
	SWProperty* prop = NULL;
	{
		ObjectMap::iterator itor = m_propTable.find( name );
		if ( itor == m_propTable.end() ) return;

		prop = swrtti_cast<SWProperty>( (itor->second)() );
	}
	if ( prop == NULL ) return;
	
	{
		SWObjectList::iterator itor = prop->m_setDelegates.begin();
		while ( itor != prop->m_setDelegates.end() )
		{
			SWDelegate* del = swrtti_cast<SWDelegate>( (*itor)() );
			if ( del == NULL || !del->isValid() )
			{
				itor = prop->m_setDelegates.erase( itor );
			}
			else ++itor;
		}
	}
}

void SWGameObject::sendMessage( const tstring& msgName, SWObject* param )
{
	SWObjectArray copy = m_components;
	SWObjectArray::iterator itor = copy.end();
	
	for ( ; itor != copy.end() ; ++itor )
	{
		SWBehavior* comp = swrtti_cast<SWBehavior>( (*itor)() );
		if ( comp == NULL ) continue;
		comp->callMessageReceiver( msgName, param );
	}
}