//
//  SWTransform.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWTransform.h"
#include "SWLog.h"

SWTransform::SWTransform( const SWVector2f& kPos, float kRot, const SWVector2f& kScale )
: pos( kPos )
, rot( kRot )
, scale( kScale )
{
    
}

SWTransform::~SWTransform()
{
    //SW_OutputLog( "transform", "deleted" );
}