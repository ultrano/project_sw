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
#include "SWMatrix4x4.h"

/**
 @brief 객체가 공간 개념을 갖게 한다.
        특별한 경우를 제외하고 가장 기본이 되는 컴포넌트
 */
class SWTransform : public SWComponent
{
    SW_RTTI( SWTransform, SWComponent );
private:

	SWWeakRef<SWTransform> m_parent;
	SWObjectList           m_children;
	SWObjectList           m_setParentDelegates;
	
	SWVector3f    m_scale;    //< 비율.
	SWQuaternion  m_rotate;   //< 회전.
	SWVector3f    m_position; //< 위치.
	SWMatrix4x4   m_worldMat; //< world matrix

public:

    SWTransform();
    ~SWTransform();

	void setParent( SWTransform* parent );
	SWTransform* getParent();
	void addSetParentDelegate( SWDelegate* callBack );
	void removeSetParentDelegate( SWDelegate* callBack );
	void removeSetParentDelegate( SWObject* object, const SWHandler& handler );

	const SWMatrix4x4& getWorldMatrix();
	SWMatrix4x4        getLocalMatrix();

	const SWVector3f&   getLocalScale();
	const SWQuaternion& getLocalRotate();
	const SWVector3f&   getLocalPosition();

	void setLocalScale( const SWVector3f& scale );
	void setLocalRotate( const SWQuaternion& rotate );
	void setLocalPosition( const SWVector3f& position );

	SWTransform* find( const tstring& name );
	void copyChildren( SWObjectList& transList );

	void onStart();
	void onRemove();
	void onUpdate( SWGameObject* );
};

#endif
