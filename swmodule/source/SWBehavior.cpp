﻿//
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