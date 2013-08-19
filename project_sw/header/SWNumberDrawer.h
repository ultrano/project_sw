//
//  SWNumberDrawer.h
//  shakeThatAss
//
//  Created by 한상운 on 13. 6. 17..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef shakeThatAss_SWNumberDrawer_h
#define shakeThatAss_SWNumberDrawer_h

#include "SWImageDrawer.h"

class SWNumberDrawer : public SWImageDrawer
{
    SW_RTTI( SWNumberDrawer, SWImageDrawer );
    
    enum { MaxNumber = 10 };
    
    int m_number;
    int m_align;
    SWImageRegion m_numRegions[MaxNumber];
    
public:
    
    SWNumberDrawer();
    
    void setAlign( int align ) { m_align = align; };
    int  getAlign() const { return m_align; };
    
    void setNumber( int number ) { m_number = number; };
    int  getNumber() const { return m_number; };
    
    void setNumberRegion( unsigned int num, const SWImageRegion& region );
    const SWImageRegion& getNumberRegion( unsigned int num );
    
    void onDraw();
};

#endif
