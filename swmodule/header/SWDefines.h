//
//  SWDefines.h
//  prototype
//
//  Created by SangWoon-Han on 13. 5. 23..
//  Copyright (c) 2013 SangWoon-Han. All rights reserved.
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
    SW_Align_Top,
	SW_Align_Bottom,

	//! shader reserved attribute index
	SW_Attribute_Position = 0,
	SW_Attribute_Texture  = 1,

	//! input stream 
	SW_Pendding_Stream = -2,
	SW_Invalid_Stream = -1,

	//! camera clear flags
	SW_Dont_Clear  = 1 << 0,
	SW_Clear_Color = 1 << 1,
	SW_Clear_Depth = 1 << 2,

	//! file mode
	SW_File_Text    = 1 << 0,
	SW_File_Binary  = 1 << 1,
	SW_File_Append  = 1 << 2,
	SW_File_Refresh = 1 << 3,

	//! maximum count of layers
	SW_MaxLayerCount = 32,

	//! layer indices, (0...31) -> (0...SW_MaxLayerCount-1)
	SW_Layer1 = 0,
	SW_Layer2 = 1,
	SW_Layer3 = 2,
	SW_DefaultLayer = SW_Layer1,

};

#define SW_PI (3.14f)
#define SW_Layer(index) (index>=SW_MaxLayerCount)? SW_MaxLayerCount:(index<0)?0:index;

#endif
