//
//  SWDefines.h
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWDefines_h
#define prototype_SWDefines_h

/** @brief 게임 내에서 사용되는 상수들의 모음 */
enum SWDefines
{
    SW_None          ,
    
    //! 터치 관련.
    SW_TouchPress    ,
    SW_TouchMove     ,
    SW_TouchRelease  ,
	SW_TouchNone     ,
    
    //! 형태 관련.
    SW_Circle        ,
    SW_Rectangle     ,
    SW_Line          ,
    SW_FillRectangle ,
    
    //! 사각 판정 관련.
    SW_RectIn        ,
    SW_RectOut       ,
    SW_RectRound     ,
    
    //! 충돌 관련 (아직 안쓰임).
    SW_CollisionEnter  ,
    SW_CollisionStay   ,
    SW_CollisionExit   ,
    
    //! 폰트 관련.
    SW_FontSize_Small   ,
    SW_FontSize_Medium  ,
    SW_FontSize_Large   ,
    SW_FontSize_Variable  ,
    
    SW_Stretch_Width,
    SW_Stretch_Height,
    SW_Stretch_Origin,
    
    //! 상태 관련.
    SW_Playing,
    SW_Stopped,
    SW_Paused,
    
    SW_Align_Left,
    SW_Align_Right,
    SW_Align_Center,
    
	//! shader reserved attribute index
	SW_Attribute_Position = 0,
	SW_Attribute_Texture  = 1,

	//! input stream 
	SW_Pendding_Stream = -2,
	SW_Invalid_Stream = -1,
};


#endif
