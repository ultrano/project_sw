//
//  SWGameScene.h
//  prototype
//
//  Created by SangWoon-Han on 13. 5. 23..
//  Copyright (c) 2013 SangWoon-Han. All rights reserved.
//

#ifndef prototype_SWGameScene_h
#define prototype_SWGameScene_h

#include "SWObject.h"

class SWGameObject;
class SWComponent;
class SWCollisionManager;

/**
 @brief 게임 플로우의 기본 단위인 scene(장면)을 관리.
 */
class SWGameScene : public SWObject
{
    SW_RTTI( SWGameScene, SWObject );
public:
    
    //! 메소드들
    SWGameScene();
    ~SWGameScene();
    
    //! 객체를 찾는다.
    SWGameObject* findGO( const tstring& name );
	void reserveDestroy( const SWGameObject* go );
    
private:

    //! @breif 처음 시작할때만 한번 호출
    virtual void onAwake() {};
    
    //! @breif 앱이 종료될때 호출
    virtual void onDestroy() {};
    
    //! @breif 앱이 갱신될때 호출
    virtual void onResume() {};
    
    //! @breif 앱이 중지될대 호출
    virtual void onPause() {};
    
    //! @breif 매프레임 갱신시에 호출
    virtual void onUpdate() {};

	//! update processing for fixed frame rate
	virtual void onFixedRateUpdate() {};
    
    //! @breif 정해진 FPS 마다 화면 갱신시에 호출.
    virtual void onPostDraw() {};
    
    //! @breif 터치 이벤트가 발생하면 호출
	virtual void onHandleTouch() {};

private:

	friend class SWTransform;
	friend class SWCamera;
	friend class SWRenderer;
	friend class SWGameContext;

	void awake();
	void destroy();
	void resume();
	void pause();
	void update();
	void fixedRateUpdate();
	void postUpdate();
	void draw();
	void handleEvent();

private:

	typedef ttable<thashstr,SWObject::List> LayerTable;

	//! root game objects
	SWObject::List m_roots;
	SWObject::List m_updates;
	SWObject::List m_cameras;
	SWObject::List m_renderers;
	SWObject::List m_destroyGOs;
	LayerTable m_layers;

	//! for fixed frame rate update
	tfloat m_fixedFrameRate;
	tfloat m_accumFrameRate;
};

#endif
