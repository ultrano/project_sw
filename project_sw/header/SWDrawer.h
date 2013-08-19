//
//  SWDrawer.h
//  prototype
//
//  Created by 한상운 on 13. 5. 24..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWDrawer_h
#define prototype_SWDrawer_h

#include "SWComponent.h"

/**
 @brief 정해진 FPS 마다 화면 갱신시에 호출.
 */
class SWDrawer : public SWComponent
{
    SW_RTTI( SWDrawer, SWComponent );
    
protected:
    
    //! 모든 드로우가 준수 해줘야 하는 속성값들은
    //! 공통 뿌리 클레스에 위치 시킨다.
    //! 하위 드로우들은 알파값에 대한 구현 처리를 준수 해야 한다.
    float m_alpha; //< 알파값
    
public:
    
    SWDrawer();
    ~SWDrawer();
    
    void setAlpha( float alpha ) { if ( alpha<=1 && alpha>=0 ) m_alpha = alpha; };
    float getAlpha() const { return m_alpha; };
    
    virtual void onDraw() = 0;
};

#endif
