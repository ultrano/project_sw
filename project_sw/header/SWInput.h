#ifndef SWInput_h__
#define SWInput_h__

#include "SWMemory.h"
#include "SWList.h"

#define SWInput (__SWInput::getInstance())

class __SWInput : public SWMemory
{
	friend class SWGameContext;
private:

	int m_touchState;
	int m_touchX;
	int m_touchY;
	SWList::Value m_listeners;

	__SWInput();
	~__SWInput();

public:

	static __SWInput& getInstance();

	int getTouchState() const;
	int getTouchX() const;
	int getTouchY() const;

	void addInputDelegate( SWDelegate* del );
	void removeInputDelegate( SWDelegate* del );
};

#endif // SWInput_h__