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

SWBehavior::~SWBehavior()
{
    //SW_OutputLog( "behavior", "deleted" );
}

void SWBehavior::update( SWGameObject* )
{
	onUpdate();
}

void SWBehavior::onStart()
{
	__super::onStart();
	gameObject()->addUpdateDelegate( GetDelegate( update ) );
}

void SWBehavior::onRemove()
{
	gameObject()->removeUpdateDelegate( GetDelegate( update ) );
	__super::onRemove();
}

void SWBehavior::setMessageReceiver( const tstring& msgName, const SWDelegate* del )
{
	m_recvTable.insert( std::make_pair( msgName, del ) );
}

const SWDelegate* SWBehavior::getMessageReceiver( const tstring& msgName )
{
	ReceiverTable::iterator itor = m_recvTable.find( msgName );
	if ( itor != m_recvTable.end() ) return itor->second();
	return NULL;
}

void SWBehavior::callMessageReceiver( const tstring& msgName, SWObject* param )
{
	const SWDelegate* del = getMessageReceiver( msgName );
	if ( del == NULL ) return;
	del->call( param );
}