﻿//
//  SWComponent.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 27..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWComponent.h"
#include "SWGameObject.h"

SWComponent::SWComponent()
{
    
}

SWComponent::~SWComponent()
{
    
}

SWComponent* SWComponent::getComponent( const SWRtti* rtti ) const
{
	if ( !gameObject.isValid() ) return NULL;
	return gameObject()->getComponent( rtti );
}

void SWComponent::destroy()
{
	SWHardRef<SWComponent> holder = this;
	if ( holder.isUndead() || !holder.isValid() ) return;

	if ( gameObject.isUndead() || !gameObject.isValid() ) return;

	gameObject()->removeComponent( queryRtti() );

	__super::destroy();
}
