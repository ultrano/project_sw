//
//  SWType.h
//  prototype
//
//  Created by 한상운 on 13. 5. 25..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef sw_SWType_h
#define sw_SWType_h

#include "SWPrimaryType.h"
#include "TVector2f.h"
#include "TVector3f.h"
#include "TQuaternion.h"
#include "TMatrix4x4.h"
#include "TMatrix3x3.h"
#include "THashString.h"
#include "TTriangle.h"
#include "TIndex3.h"
#include "TRect.h"
#include "TRay.h"
#include "TColorRGBA.h"
#include "TAABB.h"
#include "TOBB.h"

typedef TVector2f   tvec2;
typedef TVector3f   tvec3;
typedef TQuaternion tquat;
typedef TMatrix4x4  tmat44;
typedef TMatrix3x3  tmat33;
typedef THashString thashstr;
typedef TTriangle   ttriangle;
typedef TIndex3     tindex3;
typedef TRect       trect;
typedef TRay        tray;
typedef TColorRGBA  tcolor;
typedef TAABB2D     taabb2d;
typedef TAABB3D     taabb3d;
typedef TOBB2D      tobb2d;

#endif //! sw_SWType_h
