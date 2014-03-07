//
//  SWTransform.h
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWTransform_h
#define prototype_SWTransform_h

#include "SWComponent.h"
#include "TMatrix4x4.h"

/**
 @brief 객체가 공간 개념을 갖게 한다.
        특별한 경우를 제외하고 가장 기본이 되는 컴포넌트
 */
class SWTransform : public SWComponent
{
    SW_RTTI( SWTransform, SWComponent );
private:

	SWWeakRef<SWTransform> m_parent;
	SWObject::List           m_children;
	SWObject::List           m_setParentDelegates;
	
	TVector3f    m_scale;    //< 비율.
	TQuaternion  m_rotate;   //< 회전.
	TVector3f    m_euler;
	TVector3f    m_position; //< 위치.
	TMatrix4x4   m_worldMat; //< world matrix

public:

    SWTransform();
    ~SWTransform();

	void setParent( SWTransform* parent );
	SWTransform* getParent();
	void addSetParentDelegate( SWDelegator* callBack );
	void removeSetParentDelegate( SWDelegator* callBack );
	void removeSetParentDelegate( SWObject* object, const SWHandler& handler );

	const TMatrix4x4& getWorldMatrix();
	TMatrix4x4        getLocalMatrix();

	const TVector3f&   getLocalScale();
	const TQuaternion& getLocalRotate();
	const TVector3f&   getLocalPosition();

	void setLocalScale( const TVector3f& scale );
	void setLocalRotate( const TQuaternion& rotate );
	void setLocalPosition( const TVector3f& position );

	SWTransform* find( const tstring& name );
	void copyChildren( SWObject::List& transList );

	void onStart();
	void onRemove();
	void onUpdate( SWGameObject* );
	void onAnimate( const thashstr& key, float value );
};

#endif
