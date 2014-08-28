//
//  SWBehavior.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 27..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWBehavior.h"
#include "SWLog.h"
#include "SWGameObject.h"

SWBehavior::SWBehavior()
{
    
}

SWBehavior::SWBehavior( factory_constructor )
{

}

SWBehavior::~SWBehavior()
{
    //SW_OutputLog( "behavior", "deleted" );
}

void SWBehavior::update()
{
	onUpdate();
}

void SWBehavior::fixedRateUpdate()
{
	onFixedRateUpdate();
}

void SWBehavior::collision( SWObject* coll )
{
	onCollision( (SWCollision2D*)coll );
}

void SWBehavior::onAwake()
{
	__super::onAwake();
	gameObject()->addUpdateDelegator( GetDelegator( update ) );
	gameObject()->addFixedRateUpdateDelegator( GetDelegator( fixedRateUpdate ) );
	setMessageDelegator( "onCollision", GetDelegator( collision ) );
}

void SWBehavior::onRemove()
{
	gameObject()->removeFixedRateUpdateDelegator( GetDelegator( fixedRateUpdate ) );
	gameObject()->removeUpdateDelegator( GetDelegator( update ) );
	__super::onRemove();
}

void SWBehavior::onUpdate()
{

}

void SWBehavior::onFixedRateUpdate()
{

}

void SWBehavior::onCollision( SWCollision2D* )
{

}

void SWBehavior::setMessageDelegator( const tstring& msgName, const SWDelegator* del )
{
	m_recvTable.insert( std::make_pair( msgName, del ) );
}

const SWDelegator* SWBehavior::getMessageDelegator( const tstring& msgName )
{
	ReceiverTable::iterator itor = m_recvTable.find( msgName );
	if ( itor != m_recvTable.end() ) return itor->second();
	return NULL;
}

void SWBehavior::delegateMessage( const tstring& msgName, SWObject* param )
{
	const SWDelegator* del = getMessageDelegator( msgName );
	if ( del == NULL ) return;
	del->call( param );
}