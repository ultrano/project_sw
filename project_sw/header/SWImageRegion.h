//
//  SWImageRegion.h
//  protopang
//
//  Created by 한상운 on 13. 6. 5..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWImageRegion_h
#define protopang_SWImageRegion_h

#include "SWMemory.h"

class SWImageRegion : public SWMemory
{
public:
    int img;
    int x, y;
    int width, height;
    
    SWImageRegion();
    SWImageRegion( const int& kImg, int kX, int kY, int kWidth, int kHeight );
    SWImageRegion( const char* filePath, int kX, int kY, int kWidth, int kHeight );
    bool loadImage( const char* filePath, int kX, int kY, int kWidth, int kHeight );
    
};

#endif
