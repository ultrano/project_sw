//
//  SWGameObject.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWGameObject.h"
#include "SWGameScene.h"
#include "SWGameContext.h"
#include "SWTransform.h"
#include "SWBehavior.h"
#include "SWLog.h"
#include "SWProfiler.h"
#include "SWObjectStream.h"
#include "SWDefines.h"
#include "SWRefNode.h"

#include <algorithm>

class SWProperty : public SWObject
{
public:
	SWObject::Ref m_value;
};

SWGameObject::SWGameObject()
	: m_name( "nonamed" )
	, m_active( true )
	, m_layer( 0 )
{
	addComponent<SWTransform>();
}

SWGameObject::SWGameObject( factory_constructor )
	: m_name( "nonamed" )
	, m_active( true )
	, m_layer( 0 )
{
	//! don't add transform component
	//! when created by factory.
	//! because transform will be added by loader from data.
}

SWGameObject::SWGameObject( const tstring& name )
	: m_name( name )
	, m_active( true )
	, m_layer( 0 )
{
	addComponent<SWTransform>();
}

SWGameObject::~SWGameObject()
{
	SWLog( "delete game object [%s:%d]", m_name.c_str(), getID() );
}

void SWGameObject::destroy()
{
	SW_GC.getScene()->reserveDestroy( this );
}

void SWGameObject::destroyNow()
{
	SWGameObject::Ref holder = this;
	removeComponentAll();
	m_node()->gameObject = NULL;
	m_node = NULL;
	__super::destroy();
}

void SWGameObject::preUpdate()
{
	if ( m_addedComponents.size() )
	{
		m_iterateCopy = m_addedComponents;
		m_addedComponents.clear();
		SWObject::Array::iterator itor = m_iterateCopy.begin();
		for ( ; itor != m_iterateCopy.end() ; ++itor )
		{
			SWComponent* comp = swrtti_cast<SWComponent>( (*itor)() );
			comp->onStart();
		}
	}
}

void SWGameObject::udpate()
{
	preUpdate();

	if ( m_updateDelegates.size() )
	{
		SWWeakRef<SWGameObject> vital = this;
		m_iterateCopy = m_updateDelegates;
		SWObject::Array::iterator itor = m_iterateCopy.end();
		while ( itor != m_iterateCopy.begin() )
		{
			--itor;
			SWDelegator* itorDG = swrtti_cast<SWDelegator>( (*itor)() );
			if ( !itorDG || !itorDG->isValid() ) removeUpdateDelegator( itorDG );
			else itorDG->call( this );
			if ( !vital.isValid() ) return;
		}
	}
}

void SWGameObject::fixedRateUpdate()
{
	preUpdate();

	if ( m_fixedRateUpdateDelegates.size() )
	{
		SWWeakRef<SWGameObject> vital = this;
		m_iterateCopy = m_fixedRateUpdateDelegates;
		SWObject::Array::iterator itor = m_iterateCopy.end();
		while ( itor != m_iterateCopy.begin() )
		{
			--itor;
			SWDelegator* itorDG = swrtti_cast<SWDelegator>( (*itor)() );
			if ( !itorDG || !itorDG->isValid() ) removeFixedRateUpdateDelegator( itorDG );
			else itorDG->call( this );
			if ( !vital.isValid() ) return;
		}
	}
}

SWComponent* SWGameObject::addComponent( const SWRtti* rtti )
{
	if ( SWComponent* existComp = getComponent( rtti ) ) return existComp;

	SWComponent* comp = NULL;
	for ( tuint i = 0 ; i < m_loadedComponents.size() ; ++i )
	{
		comp = swrtti_cast<SWComponent>( (m_loadedComponents[i])() );
		if ( comp->queryRtti() == rtti ) break; 
		comp = NULL;
	}

	SWHardRef<SWObject> holder = NULL;
	if ( comp == NULL )
	{
		holder = SW_GC.newInstance( rtti );
		comp = swrtti_cast<SWComponent>( holder() );
		if ( comp == NULL ) return NULL;
	}

	registerComponent( comp );
	return comp;
}

void SWGameObject::registerComponent( SWComponent* comp )
{
	if ( !comp ) return;
	if ( getComponent( comp->queryRtti() ) != NULL ) return;

	m_addedComponents.push_back( comp );
	m_components.push_back( comp );
	comp->gameObject = this;
	comp->onAwake();
}

SWComponent* SWGameObject::getComponent( const SWRtti* rtti ) const
{
	if ( !rtti ) return NULL;

	for ( tuint i = 0 ; i < m_components.size() ; ++i )
	{
		SWComponent* comp = swrtti_cast<SWComponent>( (m_components[i])() );
		const SWRtti* queryRtti = comp->queryRtti();
		for ( ; queryRtti ; queryRtti = queryRtti->super )
		{
			if ( queryRtti == rtti ) return comp;
		}
	}
	return NULL;
}

SWComponent* SWGameObject::getComponent( const tstring& typeName ) const
{
	if ( typeName.size() == 0 ) return NULL;

	for ( tuint i = 0 ; i < m_components.size() ; ++i )
	{
		SWComponent* comp = swrtti_cast<SWComponent>( (m_components[i])() );
		const SWRtti* queryRtti = comp->queryRtti();
		for ( ; queryRtti ; queryRtti = queryRtti->super )
		{
			if ( queryRtti->name == typeName ) return comp;
		}
	}
	return NULL;
}

void SWGameObject::removeComponent( const SWRtti* rtti )
{
	SWComponent* comp = getComponent( rtti );
	if ( !comp ) return;
	if ( comp->queryRtti() == SWTransform::getRtti() ) return;
	
	comp->onRemove();

	SWObject::Array::iterator last = std::remove( m_components.begin(), m_components.end(), comp );
	m_components.erase( last, m_components.end() );
}

void SWGameObject::removeComponentAll()
{
	SWObject::Array copy = m_components;
	SWObject::Array::iterator itor = copy.end();

	m_components.clear();

	do
	{
		--itor;
		SWComponent* comp = swrtti_cast<SWComponent>( (*itor)() );
		comp->onRemove();
		comp->destroy();
	} while ( itor != copy.begin() );
}

void SWGameObject::addUpdateDelegator( SWDelegator* dg )
{
	if ( !dg ) return;
	SWObject::Array::iterator itor = m_updateDelegates.begin();
	for ( ; itor != m_updateDelegates.end() ; ++itor )
	{
		SWDelegator* itorDG = swrtti_cast<SWDelegator>( (*itor)() );
		if ( itorDG->isEqual( dg ) ) return;
	}
	m_updateDelegates.push_back( dg );
}

void SWGameObject::removeUpdateDelegator( SWDelegator* dg )
{
	if ( !dg ) return;
	SWObject::Array::iterator begin = m_updateDelegates.begin();
	SWObject::Array::iterator end   = m_updateDelegates.end();
	SWObject::Array::iterator last = std::remove( begin, end, dg );
	m_updateDelegates.erase( last, end );
}

tuint SWGameObject::getUpdateDelegatorCount() const
{
	return m_updateDelegates.size();
}

void SWGameObject::addFixedRateUpdateDelegator( SWDelegator* dg )
{
	if ( !dg ) return;
	SWObject::Array::iterator itor = m_fixedRateUpdateDelegates.begin();
	for ( ; itor != m_fixedRateUpdateDelegates.end() ; ++itor )
	{
		SWDelegator* itorDG = swrtti_cast<SWDelegator>( (*itor)() );
		if ( itorDG->isEqual( dg ) ) return;
	}
	m_fixedRateUpdateDelegates.push_back( dg );
}

void SWGameObject::removeFixedRateUpdateDelegator( SWDelegator* dg )
{
	if ( !dg ) return;
	SWObject::Array::iterator begin = m_fixedRateUpdateDelegates.begin();
	SWObject::Array::iterator end   = m_fixedRateUpdateDelegates.end();
	SWObject::Array::iterator last = std::remove( begin, end, dg );
	m_fixedRateUpdateDelegates.erase( last, end );
}

tuint SWGameObject::getFixedRateUpdateDelegator() const
{
	return m_fixedRateUpdateDelegates.size();
}

void SWGameObject::addLayerDelegator( SWDelegator* dg )
{
	if ( !dg ) return;
	SWObject::Array::iterator itor = m_layerDelegates.begin();
	for ( ; itor != m_layerDelegates.end() ; ++itor )
	{
		SWDelegator* itorDG = swrtti_cast<SWDelegator>( (*itor)() );
		if ( itorDG->isEqual( dg ) ) return;
	}
	m_layerDelegates.push_back( dg );
}

void SWGameObject::removeLayerDelegator( SWDelegator* dg )
{
	if ( !dg ) return;
	SWObject::Array::iterator begin = m_layerDelegates.begin();
	SWObject::Array::iterator end   = m_layerDelegates.end();
	SWObject::Array::iterator last = std::remove( begin, end, dg );
	m_layerDelegates.erase( last, end );
}

tuint SWGameObject::getLayerDelegator() const
{
	return m_layerDelegates.size();
}

void SWGameObject::sendMessage( const thashstr& msgName, SWObject* param )
{
	SWObject::Array copy = m_components;
	SWObject::Array::iterator itor = copy.begin();
	
	for ( ; itor != copy.end() ; ++itor )
	{
		SWBehavior* comp = swrtti_cast<SWBehavior>( (*itor)() );
		if ( comp == NULL ) continue;
		comp->delegateMessage( msgName, param );
	}
}

void SWGameObject::serialize( SWObjectWriter* writer )
{
	writer->writeString( m_name.str() );
	writer->writeBool( m_active );

	writer->writeUInt( m_components.size() );
	for ( tuint i = 0 ; i < m_components.size() ; ++i )
	{
		SWObject* object = (m_components[i])();
		writer->writeObject( object );
	}
}

void SWGameObject::deserialize( SWObjectReader* reader )
{
	{
		tstring name;
		reader->readString( name );
		m_name = name;
		m_active = reader->readBool();
	}

	m_loadedComponents.resize( reader->readUInt() );
	for ( tuint i = 0 ; i < m_loadedComponents.size() ; ++i )
	{
		m_loadedComponents[i] = reader->readObject();
	}

	for ( tuint i = 0 ; i < m_loadedComponents.size() ; ++i )
	{
		SWComponent* comp = swrtti_cast<SWComponent>( m_loadedComponents[i]() );
		registerComponent( comp );
	}
	m_loadedComponents.clear();
}

tuint SWGameObject::getLayer() const
{
	return m_layer;
}

void SWGameObject::setLayer( tuint layer )
{
	m_layer = layer;

	if ( m_layerDelegates.size() )
	{
		SWWeakRef<SWGameObject> vital = this;
		m_iterateCopy = m_layerDelegates;
		SWObject::Array::iterator itor = m_iterateCopy.begin();
		while ( itor != m_iterateCopy.end() )
		{
			++itor;
			SWDelegator* itorDG = swrtti_cast<SWDelegator>( (*itor)() );
			if ( !itorDG || !itorDG->isValid() ) removeLayerDelegator( itorDG );
			else itorDG->call( this );
			if ( !vital.isValid() ) return;
		}
	}
}

void SWGameObject::setActive( bool active )
{
	m_active = active;
}

bool SWGameObject::isActiveSelf() const
{
	return m_active;
}

bool SWGameObject::isActiveInScene() const
{
	SWTransform* transform = getComponent<SWTransform>();
	do 
	{
		bool active = transform->gameObject()->isActiveSelf();
		if ( active == false ) return false;
		transform = transform->getParent();

	} while ( transform != NULL );

	return true;
}
